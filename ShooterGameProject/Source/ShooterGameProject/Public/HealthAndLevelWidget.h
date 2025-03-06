#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthAndLevelWidget.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UHealthAndLevelWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Anim_Jiggle;
	void UpdateHealth(float CurrentHP, float MaxHP);
	void UpdateLevelAndExp(int32 Level, float Exp, float MaxExp);
};
