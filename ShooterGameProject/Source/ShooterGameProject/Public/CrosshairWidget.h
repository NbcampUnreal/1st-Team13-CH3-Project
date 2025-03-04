#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnFire();

	UFUNCTION(BlueprintImplementableEvent)
	void OnNormalHit();
};
