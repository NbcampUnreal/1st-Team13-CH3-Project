#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapWidget.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateMap();

};
