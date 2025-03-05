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
	class UBorder* MessageBorder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MessageText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* YesButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* NoButtonText;

	struct FBShopItemRow* PurchaseItemRow;

	class ABBaseItem* ItemToSell = nullptr;
	
	UFUNCTION(BlueprintCallable)
	void DisplayPlayerCoin();
	UFUNCTION(BlueprintCallable)
	void AddItemsToScrollBox();

	UFUNCTION(BlueprintCallable)
	void ConfirmPurchase(const FName& RowName);
	UFUNCTION(BlueprintCallable)
	void ExecutePurchase();
	UFUNCTION(BlueprintCallable)
	void ClosePurchase();

	UFUNCTION(BlueprintCallable)
	void ShowInventory();
	UFUNCTION(BlueprintCallable)
	void ConfirmSell();
	UFUNCTION(BlueprintCallable)
	void ExecuteSell();
	UFUNCTION(BlueprintCallable)
	void CancelSell();
};
