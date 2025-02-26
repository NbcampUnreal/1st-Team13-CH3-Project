#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BUIWeaponWheel.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UBUIWeaponWheel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Plays a fade-out animation and remove self from parent when animation is finished (WBP)
	UFUNCTION(BlueprintImplementableEvent)
	void ExitWeaponWheel();
};
