#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BUIManager.h"
#include "BGameInstance.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UBGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	UBGameInstance();
	virtual void Init() override;
	UPROPERTY(BlueprintReadWrite)
	bool HasTitleScreenBeenShown = false;

protected:
	UPROPERTY()
	UBUIManager* UIManagerInstance;

public:
	UFUNCTION(BlueprintPure)
	UBUIManager* GetUIManagerInstance();

	int32 GetCurrentStage() const { return CurrentStage; }
	void SetCurrentStage(int32 Stage) { CurrentStage = Stage; }

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Points);
	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetScore() const;

private:
	int32 CurrentStage = 0;
	int32 TotalScore = 0;
};
