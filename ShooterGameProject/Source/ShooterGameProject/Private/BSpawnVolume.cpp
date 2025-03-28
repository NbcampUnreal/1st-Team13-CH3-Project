#include "BSpawnVolume.h"
#include "Components/BoxComponent.h"

ABSpawnVolume::ABSpawnVolume()
{
    PrimaryActorTick.bCanEverTick = false;

    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
    SpawningBox->SetupAttachment(Scene);

}

FVector ABSpawnVolume::GetRandomPointInVolume() const
{
    FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
    FVector BoxOrigin = SpawningBox->GetComponentLocation();

    FVector SpawnLocation = BoxOrigin + FVector(
        FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
        FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
        FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z));

    return SpawnLocation;
}

AActor* ABSpawnVolume::SpawnKeyBox(TSubclassOf<AActor> ItemClass)
{
    if (!ItemClass)
    {
        UE_LOG(LogTemp, Error, TEXT("ItemClass is NULL in SpawnKeyBox!"));
        return nullptr;
    }

    FVector SpawnLocation = GetRandomPointInVolume();
    AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ItemClass, SpawnLocation, FRotator::ZeroRotator);

    return SpawnedActor;
}
