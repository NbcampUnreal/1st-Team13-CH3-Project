#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapIconWidget.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UMapIconWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MapIcon")
	AActor* IconOwner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "MapIcon")
	class UImage* MapIconImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapIcon")
	class UMapWidget* MinimapWidget;

	UFUNCTION(BlueprintCallable)
	void DetermineLocationOnMinimap(FVector2D CharacterLoc, FVector2D IconLoc);
};
