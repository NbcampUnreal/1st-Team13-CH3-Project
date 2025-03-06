#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemStruct.h"
#include "BShopRowWidget.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UBShopRowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Info;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBShopWidget* ShopWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RowName;

	UPROPERTY()
	class ABBaseItem* InventoryItemRef;
	FName InventoryItemName;
	int32 SellPrice;

	UFUNCTION(BlueprintCallable)
	void SetWidgetValues(const FBShopItemRow& ItemRow, float PriceMultiplier = 1.f);
	UFUNCTION(BlueprintCallable)
	void NotifyClicked();
};
