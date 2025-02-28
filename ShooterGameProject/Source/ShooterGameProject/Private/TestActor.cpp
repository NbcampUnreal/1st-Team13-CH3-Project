#include "TestActor.h"
#include "Components/StaticMeshComponent.h"

ATestActor::ATestActor()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // 기본 충돌 설정
    MeshComponent->SetSimulatePhysics(true);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComponent->SetCollisionObjectType(ECC_WorldStatic);
}

void ATestActor::BeginPlay()
{
    Super::BeginPlay();

}

void ATestActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
