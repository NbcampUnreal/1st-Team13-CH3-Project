#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BShopWidget.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UBShopWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TSubclassOf<UUserWidget> ShopRowWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	class UDataTable* ShopDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ShopTitle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ShopScrollBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* PlayerCoinText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* SellScrollBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SellPriceMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UBorder* MessageBorder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MessageText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* YesButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* NoButtonText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBuying = true;

	struct FBShopItemRow* PurchaseItemRow;
	class ABBaseItem* SellItemRef;
	FName SellItemName;
	int32 SellItemPrice;
	
	UFUNCTION(BlueprintCallable)
	void DisplayPlayerCoin();
	UFUNCTION(BlueprintCallable)
	void AddItemsToScrollBox();

	UFUNCTION(BlueprintCallable)
	void ConfirmPurchase(const FName& RowName);
	UFUNCTION(BlueprintCallable)
	void ExecutePurchase();
	UFUNCTION(BlueprintCallable)
	void CloseMessage();

	UFUNCTION(BlueprintCallable)
	void AddInventoryItemsToScrollBox();
	UFUNCTION(BlueprintCallable)
	void ConfirmSell(const FName& ItemName, class ABBaseItem* ItemRef, int32 SellPrice);
	UFUNCTION(BlueprintCallable)
	void ExecuteSell();
};
