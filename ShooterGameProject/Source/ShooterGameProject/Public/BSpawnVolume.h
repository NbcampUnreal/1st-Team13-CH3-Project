#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BSpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class SHOOTERGAMEPROJECT_API ABSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ABSpawnVolume();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;

	FVector GetRandomPointInVolume() const;
	AActor* SpawnKeyBox(TSubclassOf<AActor> ItemClass);

};
