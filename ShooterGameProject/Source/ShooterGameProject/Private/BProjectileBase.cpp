#include "BProjectileBase.h"
#include "Components/SphereComponent.h"
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

    // ✅ 총알 이동 설정
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 2000.0f;  // 초기 속도
    ProjectileMovement->MaxSpeed = 2000.0f;
    // 🔹 총알이 중력 영향을 받지 않도록 설정
    ProjectileMovement->ProjectileGravityScale = 0.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;  // 이동 방향으로 회전
    ProjectileMovement->bShouldBounce = false;  // 튕기지 않도록 설정
}

// ✅ 충돌 시 처리
void ABProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this)  // 자기 자신과 충돌 무시
    {
        UE_LOG(LogTemp, Log, TEXT("Projectile hit: %s"), *OtherActor->GetName());

        // ✅ 충돌한 액터가 적이면 데미지 적용
        UGameplayStatics::ApplyDamage(OtherActor, 10.0f, nullptr, this, nullptr);

        // ✅ 총알 제거
        Destroy();
    }
}

// ✅ 방향 설정 (발사)
void ABProjectileBase::FireInDirection(const FVector& ShootDirection)
{
    if (ProjectileMovement)
    {
        ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
    }
}
