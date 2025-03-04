#include "BProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

ABProjectileBase::ABProjectileBase()
{
    PrimaryActorTick.bCanEverTick = true;

    // ✅ 충돌 컴포넌트 추가 (총알 크기 조정)
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(5.0f);  // 충돌 범위 설정
    CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));  // 충돌 프로필 설정
    CollisionComponent->OnComponentHit.AddDynamic(this, &ABProjectileBase::OnHit);  // 충돌 이벤트 연결
    RootComponent = CollisionComponent;

    // ✅ 총알 메시 추가
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(CollisionComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // ✅ 충돌 비활성화 (충돌은 CollisionComponent가 담당)
    // ✅ 총알 이동 컴포넌트 추가
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = CollisionComponent;
    ProjectileMovement->InitialSpeed = 3000.0f;  // 초기 속도 (충분히 커야 함)
    ProjectileMovement->MaxSpeed = 5000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;

    // 🔹 발사체끼리 충돌하지 않도록 설정
    if (CollisionComponent)
    {
        CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        CollisionComponent->SetCollisionObjectType(ECC_PhysicsBody);
        CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
        CollisionComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);  // 다른 탄환과 충돌 방지
        // ✅ 캐릭터(Pawn)와 충돌 무시
        CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
    }

    // 🔹 탄환끼리 서로 충돌하지 않도록 설정
    TArray<AActor*> IgnoreActors;
    GetAttachedActors(IgnoreActors);
    for (AActor* IgnoredActor : IgnoreActors)
    {
        CollisionComponent->IgnoreActorWhenMoving(IgnoredActor, true);
    }
}

void ABProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    UE_LOG(LogTemp, Warning, TEXT("ABProjectileBase::OnHit Called!"));
    if (OtherActor && OtherActor != this && OtherComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *OtherActor->GetName());
    }
    if (OtherActor && OtherActor != this && OtherComp)
    {
        // ✅ 데미지 적용
        UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());

        // ✅ 탄흔 데칼 생성
        UMaterialInterface* BulletDecalMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/KSH/Asset/Bullet/M_BulletHole.M_BulletHole"));
        if (BulletDecalMaterial)
        {
            FVector DecalSize = FVector(5.0f, 5.0f, 5.0f);  // 크기 조정
            UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletDecalMaterial, DecalSize,
                Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), 10.0f);
            UDecalComponent* BulletDecal = UGameplayStatics::SpawnDecalAtLocation(
                GetWorld(), BulletDecalMaterial, FVector(5.0f, 5.0f, 5.0f),
                Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), 10.0f
            );
            if (BulletDecal)
            {
                BulletDecal->SetFadeScreenSize(0.01f); // 화면 멀어지면 자동 제거
                BulletDecal->SetFadeOut(5.0f, 1.0f);  // 5초 후에 1초 동안 서서히 사라짐
            }
        }
        // ✅ 충돌한 표면의 Physical Material 가져오기
        UPhysicalMaterial* PhysMaterial = Hit.PhysMaterial.IsValid() ? Hit.PhysMaterial.Get() : nullptr;
        float SurfaceValue = 0.0f;  // 기본값
        
        if (PhysMaterial)
        {
            // ✅ Surface Type 확인
            EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(PhysMaterial);
            FString MaterialName = PhysMaterial->GetName();

            UE_LOG(LogTemp, Warning, TEXT("Surface Type: %d, Physical Material: %s"),
                (int32)SurfaceType, *MaterialName);

            switch (SurfaceType)
            {
            case EPhysicalSurface::SurfaceType1:  // 금속 표면
                SurfaceValue = 0.0f;
                break;
            case EPhysicalSurface::SurfaceType2:  // 바닥 표면
                SurfaceValue = 1.0f;
                break;
            case EPhysicalSurface::SurfaceType3:  // 나무 표면
                SurfaceValue = 2.0f;
                break;
            case EPhysicalSurface::SurfaceType4:  // 콘크리트 표면
                SurfaceValue = 3.0f;
                break;
            case EPhysicalSurface::SurfaceType5:  // 적 표면
                SurfaceValue = 4.0f;
                break;
            default:
                SurfaceValue = 0.0f;
                break;
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("PhysMaterial is NULL!"));
        }

        // ✅ 오디오 컴포넌트를 생성하여 사운드 재생 및 파라미터 적용
        if (HitSoundCue)
        {
            UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(
                GetWorld(),
                HitSoundCue,
                Hit.ImpactPoint
            );

            if (AudioComponent)
            {
                UE_LOG(LogTemp, Warning, TEXT("✅ AudioComponent Created Successfully!"));

                // ✅ 사운드 큐의 파라미터 적용 (Int & Float 모두 설정)
                AudioComponent->SetIntParameter(TEXT("SurfaceValue"), (int32)SurfaceValue);
                AudioComponent->SetFloatParameter(TEXT("SurfaceValue"), SurfaceValue);

                UE_LOG(LogTemp, Warning, TEXT("🔹 Sound Parameter Applied: SurfaceValue = %i (Int) | %f (Float)"),
                    (int32)SurfaceValue, SurfaceValue);

                // ✅ 사운드 재생 시작
                AudioComponent->Play();

                // ✅ 0.1초 후 사운드가 실제로 재생되고 있는지 확인 (비동기 딜레이 사용)
                FTimerHandle TimerHandle;
                GetWorld()->GetTimerManager().SetTimer(TimerHandle, [AudioComponent]()
                    {
                        if (AudioComponent && AudioComponent->IsPlaying())
                        {
                            UE_LOG(LogTemp, Warning, TEXT("✅ Sound is Playing Successfully!"));
                        }
                        else
                        {
                            UE_LOG(LogTemp, Error, TEXT("⚠️ Sound did NOT play! Check SoundCue settings!"));
                        }
                    }, 0.1f, false);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("❌ Failed to create Audio Component!"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ HitSoundCue is NULL! Sound cannot be played!"));
        }

        // ✅ 총알 제거
        Destroy();

    }
}

// ✅ 방향 설정 (발사)
void ABProjectileBase::FireInDirection(const FVector& ShootDirection)
{
    // 🔹 발사자의 충돌 무시 설정
    if (GetOwner())
    {
        AActor* OwnerActor = GetOwner();
        if (CollisionComponent)
        {
            CollisionComponent->IgnoreActorWhenMoving(OwnerActor, true);
        }
    }
    if (ProjectileMovement)
    {
        ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
    }
    
}

