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
	FTimerHandle LevelTimerHandle;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void StartLevel();
	UFUNCTION(BlueprintCallable)
	void StartGame();
	UFUNCTION(BlueprintCallable)
	void onDoorReached();
	UFUNCTION(BlueprintCallable)
	void NextLevel();
	UFUNCTION(BlueprintCallable)
	void EndGame();
	UFUNCTION(BlueprintCallable)
	void QuitGame();
	virtual void RestartGame() override;
};
