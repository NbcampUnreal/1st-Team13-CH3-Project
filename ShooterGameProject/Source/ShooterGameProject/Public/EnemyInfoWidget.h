#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyInfoWidget.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UEnemyInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* EnemyHPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* EnemyNameText;

	UFUNCTION(BlueprintCallable)
	void UpdateEnemyInfo(const FName& EnemyType, const float& CurrentHP, const float& MaxHP);
};
