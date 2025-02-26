#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BUIWeaponWheel.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UBUIWeaponWheel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName HoveredItemName = NAME_None;

	// Plays a fade-out animation and remove self from parent when animation is finished (WBP)
	UFUNCTION(BlueprintNativeEvent)
	void ExitWeaponWheel();
	// Equip the character with the last hovered item
	void ExitWeaponWheel_Implementation();


};
