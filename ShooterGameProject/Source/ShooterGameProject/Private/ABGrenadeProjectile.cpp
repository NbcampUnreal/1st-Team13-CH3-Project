#include "ABGrenadeProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"

AABGrenadeProjectile::AABGrenadeProjectile()
{
    PrimaryActorTick.bCanEverTick = false;
    // 메시 추가 (수류탄 모델)
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeMesh"));
    RootComponent = MeshComponent;
    
    MeshComponent->SetHiddenInGame(false);
    MeshComponent->SetVisibility(true);  // 메시 보이도록 설정
    // 충돌 컴포넌트 생성
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(5.0f);  // 충돌 범위 설정
    CollisionComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    CollisionComponent->SetNotifyRigidBodyCollision(true);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  // 충돌 활성화
    CollisionComponent->SetCollisionObjectType(ECC_PhysicsBody);  // 물리 바디로 설정
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);  // 모든 오브젝트와 충돌
    // 루트 컴포넌트 설정
    CollisionComponent->SetupAttachment(MeshComponent);
    
    // 🔹 캐릭터(Pawn)와 충돌 무시
    CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

    // 🔹 소유자와 충돌 방지
    if (GetOwner())
    {
        CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
    }

    // OnHit 이벤트 바인딩
    if (CollisionComponent)
    {
        CollisionComponent->OnComponentHit.AddDynamic(this, &AABGrenadeProjectile::OnHit);
    }

    // 물리 시뮬레이션 활성화
    MeshComponent->SetCollisionObjectType(ECC_PhysicsBody); // 또는 ECC_WorldDynamic
    MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
    MeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
    MeshComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);


    // 발사체 이동 컴포넌트 추가
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = CollisionComponent;
    ProjectileMovement->InitialSpeed = 1000.0f;  // 초기 속도
    ProjectileMovement->MaxSpeed = 1000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;  // 튕기는 기능 활성화
    ProjectileMovement->Bounciness = 0.0f;     // 바운스 강도 (0~1)
    ProjectileMovement->Friction = 0.2f;       // 마찰력 (0~1)
    ProjectileMovement->bBounceAngleAffectsFriction = true; // 각도에 따라 마찰 적용

    // 일정 시간이 지나면 폭발하도록 설정
    InitialLifeSpan = ExplosionDelay;
}

void AABGrenadeProjectile::FireInDirection(const FVector& ShootDirection)
{
    if (ProjectileMovement)
    {
        ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
    }
}

void AABGrenadeProjectile::BeginPlay()
{
    Super::BeginPlay();

    if (MeshComponent)
    {
        MeshComponent->OnComponentHit.AddDynamic(this, &AABGrenadeProjectile::OnHit);
        UE_LOG(LogTemp, Warning, TEXT("✅ OnHit successfully bound!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ MeshComponent is NULL in BeginPlay!"));
    }
    // 충돌 후 3초 뒤에 폭발 함수 호출
    GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AABGrenadeProjectile::Explode, 3.0f, false);
}
void AABGrenadeProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // 충돌 시 로그 출력
    UE_LOG(LogTemp, Warning, TEXT("⚡ Grenade Hit: %s"), *Hit.GetActor()->GetName());
}
void AABGrenadeProjectile::Explode()
{
    // 폭발 위치가 (0,0,0)인지를 확인하고, 적절히 설정
    if (ExplosionLocation.IsNearlyZero())
    {
        ExplosionLocation = GetActorLocation(); // 현재 액터 위치를 사용
    }

    UE_LOG(LogTemp, Warning, TEXT("💥 Explosion at: X=%.2f Y=%.2f Z=%.2f"),
        ExplosionLocation.X, ExplosionLocation.Y, ExplosionLocation.Z);

    // 폭발 이펙트 및 데미지 적용 (기존 코드)
    if (ExplosionEffect)
    {
        UNiagaraComponent* NiagaraEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(), ExplosionEffect, ExplosionLocation, FRotator::ZeroRotator);

        if (NiagaraEffect)
        {
            // 일정 시간 후 폭발 이펙트 제거
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, [NiagaraEffect]() {
                NiagaraEffect->DestroyComponent();
                }, 3.0f, false);
        }
    }

    // 폭발 데미지 적용
    UGameplayStatics::ApplyRadialDamage(
        GetWorld(), ExplosionDamage, ExplosionLocation, ExplosionRadius,
        UDamageType::StaticClass(), TArray<AActor*>(), this, GetInstigatorController(), true);

    // 액터 제거
    Destroy();
}
