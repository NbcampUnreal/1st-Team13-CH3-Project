#include "BPistol.h"
#include "BCharacter.h"       // BCharacter 포함
#include "BGameInstance.h"
#include "Kismet/GameplayStatics.h"
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
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;  // RootComponent로 설정

    FRotator NewRotation(0.0f, 90.0f, 90.0f); // 예: Y축으로 90도 회전
    WeaponMesh->SetRelativeRotation(NewRotation);
    // ✅ WeaponMesh에 부착 (RootComponent에 부착하면 맵 중앙에 남음)
    Collision->SetupAttachment(WeaponMesh);
    // 기본 총구 위치를 설정 (이것은 예시이며, 적절한 값으로 설정할 필요 있음)
    GunMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("GunMuzzle"));
    GunMuzzle->SetupAttachment(RootComponent); // 총구 위치 설정 (각 총기마다 다를 수 있음)
    

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
    FRotator MuzzleRotation = OwnerCharacter->GetControlRotation();

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
            UE_LOG(LogTemp, Log, TEXT("총알 스폰 성공: %s"), *Projectile->GetName());
            Projectile->FireInDirection(AdjustedShootDirection);  // 🔹 조정된 방향으로 발사
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
}

void ABPistol::DisablePhysicsSimulation()
{
    if (WeaponMesh)
    {
        WeaponMesh->SetSimulatePhysics(false);
    }
}
