#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BGameMode.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API ABGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ABGameMode();
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void StartLevel();
	UFUNCTION(BlueprintCallable)
	void StartGame();
	FTimerHandle LevelTimerHandle;
	UFUNCTION(BlueprintCallable)
	void EnemyDefeated();
	UFUNCTION(BlueprintCallable)
	void ItemCollected();
	UFUNCTION(BlueprintCallable)
	void CheckGameStatus();
	UFUNCTION(BlueprintCallable)
	void OpenDoor();
	UFUNCTION(BlueprintCallable)
	void onDoorReached();
	UFUNCTION(BlueprintCallable)
	void NextLevel();
	UFUNCTION(BlueprintCallable)
	void EndGame(bool bIsVictory);
	UFUNCTION(BlueprintCallable)
	void EndGame();
	UFUNCTION(BlueprintCallable)
	void RestartGame();
};