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
	UFUNCTION(BlueprintCallable)
	void StartGame();
	void EnemyDefeated();
	void ItemCollected();
	void CheckGameStatus();
	void OpenDoor();
	void onDoorReached();
	void NextLevel();
	void EndGame(bool bIsVictory);
	void RestartGame();
};