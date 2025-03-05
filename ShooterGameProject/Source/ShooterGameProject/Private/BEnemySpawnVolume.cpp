#include "BEnemySpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BEnemyBase.h"
#include "Engine/World.h"

ABEnemySpawnVolume::ABEnemySpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	ESpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ESpawnBox"));
	ESpawnBox->SetupAttachment(Scene);
}

void ABEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();	
}

void ABEnemySpawnVolume::SpawnEnemies(int32 BasicCount, int32 SkillCount) //ÂÌ¸÷,½ºÅ³¸÷
{
    UWorld* World = GetWorld();

    for (int32 i = 0; i < BasicCount; i++)
    {
        FVector SpawnLocation = GetValidSpawnLocation();
        FRotator SpawnRotation = FRotator::ZeroRotator;
        TSubclassOf<ABEnemyBase> EnemyClass = GetRandomEnemyClass(true);

        if (EnemyClass)
        {
            World->SpawnActor<ABEnemyBase>(EnemyClass, SpawnLocation, SpawnRotation);
        }
    }

    for (int32 i = 0; i < SkillCount; i++)
    {
        FVector SpawnLocation = GetValidSpawnLocation();
        FRotator SpawnRotation = FRotator::ZeroRotator;
        TSubclassOf<ABEnemyBase> EnemyClass = GetRandomEnemyClass(false);

        if (EnemyClass)
        {
            World->SpawnActor<ABEnemyBase>(EnemyClass, SpawnLocation, SpawnRotation);
        }
    }
}

FVector ABEnemySpawnVolume::GetValidSpawnLocation()
{
    FVector Origin = ESpawnBox->Bounds.Origin;
    FVector Extent = ESpawnBox->Bounds.BoxExtent;
    FVector RandomLocation = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

    return RandomLocation;
}

TSubclassOf<ABEnemyBase> ABEnemySpawnVolume::GetRandomEnemyClass(bool bIsBasic)
{
    if (bIsBasic && BasicEnemies.Num() > 0)
    {
        return BasicEnemies[FMath::RandRange(0, BasicEnemies.Num() - 1)];
    }
    else if (!bIsBasic && SkillEnemies.Num() > 0)
    {
        return SkillEnemies[FMath::RandRange(0, SkillEnemies.Num() - 1)];
    }
    return nullptr;
}