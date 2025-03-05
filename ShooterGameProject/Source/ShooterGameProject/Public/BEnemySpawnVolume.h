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

	void SpawnEnemies(int32 BasicCount, int32 SkillCount);  //����
	TSubclassOf<ABEnemyBase> GetRandomEnemyClass(bool bIsBasic);  //���� ��ų�� ����
	FVector GetValidSpawnLocation();  //������ġ

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TArray<TSubclassOf<ABEnemyBase>> BasicEnemies;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TArray<TSubclassOf<ABEnemyBase>> SkillEnemies;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* ESpawnBox;

};
