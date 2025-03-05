#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapWidget.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMapWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
	UMaterialParameterCollection* MPCMinimap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Minimap")
	class UOverlay* MapOverlay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Minimap")
	class UImage* MapImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Minimap")
	class UImage* PlayerMarker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap|Icon")
	TSubclassOf<UUserWidget> MapIconWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
	float CaptureOffsetX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
	float CaptureOffsetY;

	void UpdateMap();
};
