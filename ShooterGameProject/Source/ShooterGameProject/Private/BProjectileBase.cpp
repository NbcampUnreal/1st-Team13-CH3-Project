#include "BProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
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
        CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        CollisionComponent->SetCollisionObjectType(ECC_PhysicsBody);

        // 다른 탄환과 충돌 무시
        CollisionComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
        CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
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

        // ✅ 총알 제거 (충돌 후 사라지게)
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

