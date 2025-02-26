#include "ABGrenadeProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AABGrenadeProjectile::AABGrenadeProjectile()
{
    PrimaryActorTick.bCanEverTick = false;
    // 루트 컴포넌트 설정
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // 메시 추가 (수류탄 모델)
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeMesh"));
    MeshComponent->SetupAttachment(RootComponent);

    
    ProjectileMovement->bShouldBounce = true; // ✅ 벽에 부딪히면 튕기도록 설정
    ProjectileMovement->Bounciness = 0.6f;
    ProjectileMovement->ProjectileGravityScale = 1.0f; // ✅ 중력 적용
    ProjectileMovement->InitialSpeed = 1000.0f;
    ProjectileMovement->MaxSpeed = 1000.0f;

    // 일정 시간이 지나면 폭발하도록 설정
    InitialLifeSpan = ExplosionDelay;
   
}
void AABGrenadeProjectile::FireInDirection(const FVector& ShootDirection)
{
    if (ProjectileMovement)
    {
        ProjectileMovement->Velocity = ShootDirection;  // 속도 그대로 적용
        Explode();
    }
}

void AABGrenadeProjectile::BeginPlay()
{
    Super::BeginPlay();

    // ExplosionDelay 후에 자동으로 Explode() 실행
    GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &AABGrenadeProjectile::Explode, ExplosionDelay, false);
}

void AABGrenadeProjectile::Explode()
{
    // 폭발 이펙트 생성
    if (ExplosionEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
    }

    // 폭발 데미지 적용 (반경 내 모든 대상에게 피해)
    UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, GetActorLocation(), ExplosionRadius,
        UDamageType::StaticClass(), TArray<AActor*>(), this, GetInstigatorController(), true);

    // 액터 제거
    Destroy();
}
