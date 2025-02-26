#include "BShotgun.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "BGameInstance.h"
#include "BUIManager.h"

#include "Components/SphereComponent.h" // SphereComponent 헤더 포함 필수

ABShotgun::ABShotgun()
{
    FireRate = 0.8f;  // 피스톨 연사 속도 설정
    WeaponType = "ShotGun";
    AmmoCount = 12;
    ShotPelletCount = 6;
    PelletSpreadAngle = 35.0f;
    Damage = 10.0f; // 기본 탄환 데미지 설정

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
    GunMuzzle->SetupAttachment(Muzzle);
}


void ABShotgun::Attack()
{
    if (!OwnerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("무기 소유 캐릭터가 없습니다!"));
        return;
    }

    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime - LastFireTime < FireRate)
    {
        UE_LOG(LogTemp, Warning, TEXT("발사 대기 중..."));
        return;
    }
    LastFireTime = CurrentTime;

    if (AmmoCount <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("탄약이 없습니다! 재장전 필요"));
        return;
    }
    AmmoCount--;

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }

    FVector MuzzleLocation = GunMuzzle ? GunMuzzle->GetComponentLocation() : GetActorLocation();
    FVector CrosshairTarget = GetCrosshairTarget(); // 크로스헤어 기준 목표 지점
    FVector ForwardVector = (CrosshairTarget - MuzzleLocation).GetSafeNormal();

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
