#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillCountWidget.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UKillCountWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateKillCount(const int32& KillCount);
	void UpdateCurrentScore(const int32& CurrentScore);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* KillCountText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CurrentScoreText;
};
