#include "BProjectileBase.h"
#include "Components/StaticMeshComponent.h"

// 생성자
ABProjectileBase::ABProjectileBase()
{
    PrimaryActorTick.bCanEverTick = true;

    // 메시 생성 및 기본 설정
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // 기본 속도 설정
    Speed = 1000.f;
}

// BeginPlay
void ABProjectileBase::BeginPlay()
{
    Super::BeginPlay();
}

void ABProjectileBase::FireInDirection(const FVector& ShootDirection)
{
    if (ProjectileMovement)
    {
        ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
    }
}

// Tick
void ABProjectileBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 총알이 이동하는 로직을 여기서 처리 (예: 일정 속도로 직선 이동)
    FVector ForwardDirection = GetActorForwardVector();
    FVector NewLocation = GetActorLocation() + (ForwardDirection * Speed * DeltaTime);
    SetActorLocation(NewLocation);
}

// 충돌 시 처리되는 함수
void ABProjectileBase::OnHit()
{
    // 충돌 처리 (예: 데미지 적용)
    UE_LOG(LogTemp, Log, TEXT("Projectile Hit!"));
}
