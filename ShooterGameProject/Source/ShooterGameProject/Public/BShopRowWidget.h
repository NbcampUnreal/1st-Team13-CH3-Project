#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BShopRowWidget.generated.h"

struct FBShopItemRow;

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


	UFUNCTION(BlueprintCallable)
	void SetWidgetValues(const FBShopItemRow& ItemRow);
	UFUNCTION(BlueprintCallable)
	void NotifyClicked();
};
