#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BEnemySpawnVolume.generated.h"

class ABEnemyBase;
class UBoxComponent;
UCLASS()
class SHOOTERGAMEPROJECT_API ABEnemySpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ABEnemySpawnVolume();

protected:
	virtual void BeginPlay() override;

public:	

	void SpawnEnemies(int32 BasicCount, int32 SkillCount);  //스폰
	TSubclassOf<ABEnemyBase> GetRandomEnemyClass(bool bIsBasic);  //랜덤 스킬몹 선택
	FVector GetValidSpawnLocation();  //스폰위치

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TArray<TSubclassOf<ABEnemyBase>> BasicEnemies;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TArray<TSubclassOf<ABEnemyBase>> SkillEnemies;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* ESpawnBox;

};
