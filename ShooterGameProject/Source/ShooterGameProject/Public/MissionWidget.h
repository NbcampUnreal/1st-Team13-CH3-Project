#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionWidget.generated.h"

class UTextBlock;
class UVerticalBox;

UCLASS()
class SHOOTERGAMEPROJECT_API UMissionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	bool bIsTitleAdded = false;
	TArray<UTextBlock*> TextBlocks;

	void UpdateItemMission(const FName& ItemName, const int32& CurrentCount, const int32& TargetCount);
	void UpdateBonusMission(const int32& CurrentCount, const int32& TargetCount);
	void RemoveAllMissions();

	UFUNCTION(BlueprintImplementableEvent)
	void OnMissionCompleted(UTextBlock* MissionText);

private:
	void AddMissionTitle(const FString& MissionTitle, UVerticalBox* MissionVBox);
};
