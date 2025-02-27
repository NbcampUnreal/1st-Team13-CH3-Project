#include "BShotgun.h"
#include "BUIManager.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"  // 🔹 UNiagaraComponent 정의 포함
#include "NiagaraFunctionLibrary.h"
#include "DrawDebugHelpers.h"
#include "BGameInstance.h"

#include "Components/SphereComponent.h" // SphereComponent 헤더 포함 필수

ABShotgun::ABShotgun()
{
    WeaponType = "ShotGun";
    AmmoCount = 12;
    ShotPelletCount = 6;
    PelletSpreadAngle = 35.0f;
    Damage = 10.0f; // 기본 탄환 데미지 설정
    bCanFire = true; // 🔹 게임 시작 시 공격 가능 상태로 설정
    // 🔹 본체 (Root Component로 설정)
    ShotGunBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShotGunBody"));
    SetRootComponent(ShotGunBody);  // 🔹 루트 컴포넌트 설정

    FRotator NewRotation(0.0f, 0.0f, -180.0f); // 방향 조정
    ShotGunBody->SetRelativeRotation(NewRotation);

    // 🔹 충돌 설정 (Collision)
     // 루트 컴포넌트로 설정
    Collision->SetupAttachment(ShotGunBody);

    // 🔹 총구 (Muzzle)
    Muzzle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Muzzle"));
    Muzzle->SetupAttachment(ShotGunBody);
    Muzzle->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));  // 총구 위치 조정

    // 🔹 총구 위치 정보 (GunMuzzle)
    GunMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("GunMuzzle"));
    GunMuzzle->SetupAttachment(ShotGunBody);

    //총기 배출구 생성
    ShellEjectSocket = CreateDefaultSubobject<USceneComponent>(TEXT("ShellEjectSocket"));
    ShellEjectSocket->SetupAttachment(RootComponent);  // 루트 컴포넌트에 부착
}


void ABShotgun::Attack()
{
    UE_LOG(LogTemp, Warning, TEXT("🔫 [ABShotgun] Attack() 실행"));

    if (!OwnerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ [ABShotgun] 무기 소유 캐릭터가 없습니다!"));
        return;
    }


    AActor* EquippedWeapon = OwnerCharacter->GetCurrentWeapon();
    UE_LOG(LogTemp, Warning, TEXT("🛠️ [ABShotgun] 현재 장착된 무기: %s"), *EquippedWeapon->GetName());

    if (EquippedWeapon != this)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ [ABShotgun] 현재 장착된 무기가 아닙니다!"));
        return;
    }

    float CurrentTime = GetWorld()->GetTimeSeconds();
    UE_LOG(LogTemp, Warning, TEXT("⏳ [ABShotgun] 현재 시간: %f, 마지막 발사 시간: %f, FireRate: %f"),
        CurrentTime, LastFireTime, FireRate);

    if (CurrentTime - LastFireTime < FireRate)
    {
        UE_LOG(LogTemp, Warning, TEXT("⏳ [ABShotgun] 발사 대기 중..."));
        return;
    }
    LastFireTime = CurrentTime;

    if (AmmoCount <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ [ABShotgun] 탄약이 없습니다! 재장전 필요"));
        return;
    }



    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }

    FVector MuzzleLocation = GunMuzzle ? GunMuzzle->GetComponentLocation() : GetActorLocation();
    // 🔹 머즐 플래시 효과 재생
    if (MuzzleFlashEffect)
    {
        FVector MuzzleFlashLocation = GunMuzzle ? GunMuzzle->GetComponentLocation()
            + (GetActorForwardVector()) // 🔹 총구에서 약간 앞으로 조정
            : GetActorLocation();
        FRotator MuzzleFlashRotation = OwnerCharacter->GetControlRotation(); // 🔹 카메라 방향을 바라보도록 설정

        UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
            MuzzleFlashEffect,
            ShotGunBody,
            TEXT("GunMuzzle"), // 소켓 이름
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::SnapToTarget,
            true
        );
        if (NiagaraComp)
        {
            // 나이아가라 파티클의 'Lifetime'을 설정
            NiagaraComp->SetFloatParameter(TEXT("Lifetime"), 0.001f); // 0.05초 지속
        }
    }
    FVector CrosshairTarget = GetCrosshairTarget();
    FVector ForwardVector = (CrosshairTarget - MuzzleLocation).GetSafeNormal();

    SplitRotation = ForwardVector.Rotation();

    UE_LOG(LogTemp, Warning, TEXT("📍 [ABShotgun] MuzzleLocation: %s"), *MuzzleLocation.ToString());
    UE_LOG(LogTemp, Warning, TEXT("🎯 [ABShotgun] CrosshairTarget: %s"), *CrosshairTarget.ToString());
    UE_LOG(LogTemp, Warning, TEXT("📐 [ABShotgun] SplitRotation: %s"), *SplitRotation.ToString());

    // 🔹 탄환 클래스가 설정되었는지 확인
    if (!ProjectileClass)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ [ABShotgun] ProjectileClass가 설정되지 않았습니다! 탄환이 생성되지 않습니다."));
        return;
    }
    if (ShellClass)
    {
        FVector ShellEjectLocation = ShellEjectSocket
            ? ShellEjectSocket->GetComponentLocation()
            + (GetActorForwardVector() * -20.0f)  // 🔹 총기 뒷부분으로 이동
            + (GetActorRightVector() * 5.0f)      // 🔹 오른쪽으로 이동
            + (GetActorUpVector() * 2.0f)         // 🔹 살짝 위로 조정
            : GetActorLocation();

        FRotator ShellEjectRotation = ShellEjectSocket
            ? ShellEjectSocket->GetComponentRotation()
            : FRotator::ZeroRotator;

        UE_LOG(LogTemp, Log, TEXT("탄피 스폰 시도: %s"), *ShellClass->GetName());

        ABBulletShell* Shell = GetWorld()->SpawnActor<ABBulletShell>(ShellClass, ShellEjectLocation, ShellEjectRotation);

        if (Shell)
        {
            UE_LOG(LogTemp, Log, TEXT("탄피 스폰 성공: %s"), *Shell->GetName());
            Shell->SetShellType("Pistol");

            FVector EjectDirection =
                (GetActorRightVector() * FMath::RandRange(2.5f, 3.0f)) +  // 🔹 오른쪽으로 더 강하게 튀게
                (GetActorUpVector() * FMath::RandRange(0.5f, 1.0f)) +    // 🔹 위로 더 튀게
                (GetActorForwardVector() * FMath::RandRange(-1.0f, -2.0f)); // 🔹 약간 뒤로 밀려나게

            Shell->GetShellMesh()->AddImpulse(EjectDirection * 30.0f);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("탄피 스폰 실패!"));
        }
        // 🔹 탄환 발사
        // 🔹 발사된 탄환을 저장할 리스트
        TArray<ABProjectileBase*> SpawnedProjectiles;

        for (int32 i = 0; i < ShotPelletCount; i++)
        {
            float RandomYaw = FMath::RandRange(-PelletSpreadAngle, PelletSpreadAngle);
            float RandomPitch = FMath::RandRange(-PelletSpreadAngle, PelletSpreadAngle);
            FRotator AdjustedRotation = ForwardVector.Rotation() + FRotator(RandomPitch, RandomYaw, 0);
            FVector ShotDirection = AdjustedRotation.Vector();

            if (ProjectileClass)
            {
                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = this;
                SpawnParams.Instigator = OwnerCharacter;
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // 안전한 스폰 설정

                ABProjectileBase* Projectile = GetWorld()->SpawnActor<ABProjectileBase>(ProjectileClass, MuzzleLocation, AdjustedRotation, SpawnParams);
                if (Projectile)
                {
                    Projectile->SetDamage(Damage); // 개별 탄환 데미지 설정
                    Projectile->FireInDirection(ShotDirection);

                    // 🔹 다른 탄환들과 충돌 무시 설정 (CollisionComponent 사용)
                    for (ABProjectileBase* SpawnedProjectile : SpawnedProjectiles)
                    {
                        if (SpawnedProjectile && Projectile)
                        {
                            SpawnedProjectile->CollisionComponent->IgnoreActorWhenMoving(Projectile, true);
                            Projectile->CollisionComponent->IgnoreActorWhenMoving(SpawnedProjectile, true);
                        }
                    }

                    // 발사된 탄환을 리스트에 저장
                    SpawnedProjectiles.Add(Projectile);
                }
            }
        }

        UE_LOG(LogTemp, Warning, TEXT("🎯 [ABShotgun] 공격 완료"));
    }
}


FVector ABShotgun::GetCrosshairTarget()
{
    UBGameInstance* GameInstance = Cast<UBGameInstance>(GetWorld()->GetGameInstance());
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("GameInstance를 찾을 수 없습니다!"));
        return FVector::ZeroVector;
    }

    UBUIManager* UIManager = GameInstance->GetUIManagerInstance();
    if (!UIManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("UBUIManager를 찾을 수 없습니다!"));
        return FVector::ZeroVector;
    }

    TTuple<FVector, FVector> CrosshairData = UIManager->GetCrosshairLocationAndDirection();
    FVector CrosshairLocation = CrosshairData.Get<0>();
    FVector CrosshairDirection = CrosshairData.Get<1>();
    FVector TraceEnd = CrosshairLocation + (CrosshairDirection * 30000.0f);

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(OwnerCharacter);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, CrosshairLocation, TraceEnd, ECC_Visibility, QueryParams))
    {
        return HitResult.ImpactPoint;
    }
    return TraceEnd;
}
