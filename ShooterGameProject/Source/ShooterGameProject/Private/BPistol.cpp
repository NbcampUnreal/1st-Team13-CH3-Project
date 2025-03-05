#include "BPistol.h"
#include "BCharacter.h"       // BCharacter 포함
#include "BGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"  // 🔹 UNiagaraComponent 정의 포함
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"



ABPistol::ABPistol()
{
    FireRate = 0.3f;  // 피스톨 연사 속도 설정
    LastFireTime = -FireRate; // 🔹 시작 시 즉시 발사 가능하도록 설정
    Damage = 20.0f;   // 피스톨 기본 데미지 설정
    // 탄약 기본값 설정
    AmmoCount = 90;
    WeaponName = "M1911";
    WeaponType = "Pistol";
    // 스태틱 메시 컴포넌트 초기화
    RootComponent = WeaponMesh;  // RootComponent로 설정

    FRotator NewRotation(0.0f, 90.0f, 90.0f); // 예: Y축으로 90도 회전
    WeaponMesh->SetRelativeRotation(NewRotation);
    // ✅ WeaponMesh에 부착 (RootComponent에 부착하면 맵 중앙에 남음)
    Collision->SetupAttachment(WeaponMesh);
    // 기본 총구 위치를 설정 (이것은 예시이며, 적절한 값으로 설정할 필요 있음)
    GunMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("GunMuzzle"));
    GunMuzzle->SetupAttachment(RootComponent); // 총구 위치 설정 (각 총기마다 다를 수 있음)
    //총기 배출구 생성
    ShellEjectSocket = CreateDefaultSubobject<USceneComponent>(TEXT("ShellEjectSocket"));
    ShellEjectSocket->SetupAttachment(RootComponent);  // 루트 컴포넌트에 부착
    // 물리 시뮬레이션 끄기
    DisablePhysicsSimulation();
}

void ABPistol::Attack()
{
    if (!OwnerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("무기 소유 캐릭터가 없습니다!"));
        return;
    }

    float CurrentTime = GetWorld()->GetTimeSeconds();

    // 🔹 FireRate 체크 (연속 발사 방지)
    if (CurrentTime - LastFireTime < FireRate)
    {
        UE_LOG(LogTemp, Warning, TEXT("발사 대기 중..."));
        return;
    }

    LastFireTime = CurrentTime; // 🔹 마지막 발사 시간 업데이트

    if (AmmoCount <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("탄약이 없습니다! 재장전 필요"));
        return;
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
            WeaponMesh,            
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
    // 🔹 크로스헤어 방향 가져오기
    UBGameInstance* GameInstance = Cast<UBGameInstance>(GetWorld()->GetGameInstance());
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("GameInstance를 찾을 수 없습니다!"));
        return;
    }

    UBUIManager* UIManager = GameInstance->GetUIManagerInstance();
    if (!UIManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("UBUIManager를 찾을 수 없습니다!"));
        return;
    }

    TTuple<FVector, FVector> CrosshairData = UIManager->GetCrosshairLocationAndDirection();
    FVector CrosshairLocation = CrosshairData.Get<0>();
    FVector CrosshairDirection = CrosshairData.Get<1>();
    FVector EndTrace = CrosshairLocation + (CrosshairDirection * 15000.0f);

    // 🔹 라인트레이스 설정
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(OwnerCharacter);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, CrosshairLocation, EndTrace, ECC_Visibility, Params))
    {
        EndTrace = HitResult.ImpactPoint;
    }

    FVector AdjustedShootDirection = (EndTrace - MuzzleLocation).GetSafeNormal();

    // 🔹 총알 스폰
    if (ProjectileClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = OwnerCharacter;

        UE_LOG(LogTemp, Log, TEXT("총알 스폰 시도: %s"), *ProjectileClass->GetName());

        // 총알 생성
        ABProjectileBase* Projectile = GetWorld()->SpawnActor<ABProjectileBase>(
            ProjectileClass, MuzzleLocation,
            FRotationMatrix::MakeFromX(AdjustedShootDirection).Rotator(), // 조정된 방향 적용
            SpawnParams
        );

        if (Projectile)
        {
            Projectile->FireInDirection(AdjustedShootDirection);  // 🔹 조정된 방향으로 발사
            Projectile->SetDamage(Damage);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ProjectileClass가 설정되지 않았습니다!"));
    }

    // 🔹 사운드 재생
    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }

    if (ShellClass)
    {
        FVector ShellEjectLocation = ShellEjectSocket
            ? ShellEjectSocket->GetComponentLocation()
            + (GetActorForwardVector() * -35.0f)  // 🔹 총기 뒷부분으로 이동
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
                (GetActorRightVector() * FMath::RandRange(4.5f, 5.0f)) +  // 🔹 오른쪽으로 더 강하게 튀게
                (GetActorUpVector() * FMath::RandRange(1.5f, 2.0f)) +    // 🔹 위로 더 튀게
                (GetActorForwardVector() * FMath::RandRange(-1.0f, -2.0f)); // 🔹 약간 뒤로 밀려나게

            Shell->GetShellMesh()->AddImpulse(EjectDirection * 30.0f); 
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("탄피 스폰 실패!"));
        }
    }

}

void ABPistol::DisablePhysicsSimulation()
{
    if (WeaponMesh)
    {
        WeaponMesh->SetSimulatePhysics(false);
    }
}
