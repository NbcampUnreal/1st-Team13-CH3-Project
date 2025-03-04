#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuickslotWidget.generated.h"


UCLASS()
class SHOOTERGAMEPROJECT_API UQuickslotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateQuickslot(const FName& ItemName, const int32& Count);
};
