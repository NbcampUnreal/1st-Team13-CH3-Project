#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthAndLevelWidget.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UHealthAndLevelWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateHealth(float CurrentHP, float MaxHP);
	void UpdateLevelAndExp(int32 Level, float Exp, float MaxExp);
};
