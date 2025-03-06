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
    CurrentAmmo = 12; // 1탄창 지급
    ReservedAmmo = 0; // 예비탄환은 0개
    ShotPelletCount = 6;
    PelletSpreadAngle = 35.0f;
    Damage = 20.0f; // 기본 탄환 데미지 설정
    bCanFire = true; // 🔹 게임 시작 시 공격 가능 상태로 설정
    // 🔹 본체 (Root Component로 설정)
    ShotGunBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShotGunBody"));
    SetRootComponent(ShotGunBody);  // 🔹 루트 컴포넌트 설정

    FRotator NewRotation(0.0f, 0.0f, -180.0f); // 방향 조정
    ShotGunBody->SetRelativeRotation(NewRotation);

    // 🔹 충돌 설정 (Collision)
     // 루트 컴포넌트로 설정
    Collision->SetupAttachment(ShotGunBody);
    // 🔹 조준경 (옵션)
    EquippedPartMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquippedPartMesh"));
    EquippedPartMesh->SetupAttachment(ShotGunBody);
    EquippedPartMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));  // 위치 조정
    // 🔹 총구 (Muzzle)
    Muzzle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Muzzle"));
    Muzzle->SetupAttachment(ShotGunBody);
    Muzzle->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));  // 총구 위치 조정

    // 🔹 총구 위치 정보 (GunMuzzle)
    GunMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("GunMuzzle"));
    GunMuzzle->SetupAttachment(RootComponent);

    //총기 배출구 생성
    ShellEjectSocket = CreateDefaultSubobject<USceneComponent>(TEXT("ShellEjectSocket"));
    ShellEjectSocket->SetupAttachment(RootComponent);  // 루트 컴포넌트에 부착
}


bool ABShotgun::IsPartMeshEquipped(ABShotgunPart* Part)
{
    // 이미 장착된 파츠의 매쉬가 있으면 새로 장착하지 않도록 처리
    return EquippedPartMesh == Part->Mesh;
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

    if (EquippedWeapon != this)
    {
        return;
    }

    float CurrentTime = GetWorld()->GetTimeSeconds();

    if (CurrentTime - LastFireTime < FireRate)
    {
        UE_LOG(LogTemp, Warning, TEXT("⏳ [ABShotgun] 발사 대기 중..."));
        return;
    }
    LastFireTime = CurrentTime;

    if (CurrentAmmo <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("탄약이 없습니다! 재장전 필요"));
        if (ReservedAmmo > 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("샷건 추가 탄환이 있으니 자동 재장전 하겠습니다."));
            Reload();
        }
        return;
    }
    else {
        // 탄약 감소
        CurrentAmmo--;
        UE_LOG(LogTemp, Error, TEXT("❌ 현재탄환: %d"), CurrentAmmo);
    }

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
        // 📌 🔊 총기 발사 소음 발생!
        MakeNoise(1.0f, OwnerCharacter, GetActorLocation());
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
            GunMuzzle,
            TEXT("GunMuzzle"), // 소켓 이름
            GunMuzzle ? GunMuzzle->GetRelativeLocation() : FVector::ZeroVector, // 🔹 총구 위치 반영
            GunMuzzle ? GunMuzzle->GetRelativeRotation() : FRotator::ZeroRotator, // 🔹 총구 회전 반영
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


    // 🔹 탄환 클래스가 설정되었는지 확인
    if (!ProjectileClass)
    {
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


        ABBulletShell* Shell = GetWorld()->SpawnActor<ABBulletShell>(ShellClass, ShellEjectLocation, ShellEjectRotation);

        if (Shell)
        {
            Shell->SetShellType("Pistol");

            FVector EjectDirection =
                (GetActorRightVector() * FMath::RandRange(2.5f, 3.0f)) +  // 🔹 오른쪽으로 더 강하게 튀게
                (GetActorUpVector() * FMath::RandRange(0.5f, 1.0f)) +    // 🔹 위로 더 튀게
                (GetActorForwardVector() * FMath::RandRange(-1.0f, -2.0f)); // 🔹 약간 뒤로 밀려나게

            Shell->GetShellMesh()->AddImpulse(EjectDirection * 30.0f);
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
                    // Generate a random value within a certain range, for example between -5 and 5
                    float RandomDamage = FMath::RandRange(-5.0f, 5.0f);

                    // Add the random value to the base damage
                    float FinalDamage = Damage + RandomDamage;
                    Projectile->SetDamage(FinalDamage); // 개별 탄환 데미지 설정
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
    }
    if (UBGameInstance* Instance = Cast<UBGameInstance>(GetGameInstance()))
    {
        if (UBUIManager* UIManager = Cast<UBUIManager>(Instance->GetUIManagerInstance()))
        {
            UIManager->UpdateHUDAmmo();
        }
    }
}


FVector ABShotgun::GetCrosshairTarget()
{
    UBGameInstance* GameInstance = Cast<UBGameInstance>(GetWorld()->GetGameInstance());
    if (!GameInstance)
    {
        return FVector::ZeroVector;
    }

    UBUIManager* UIManager = GameInstance->GetUIManagerInstance();
    if (!UIManager)
    {
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
