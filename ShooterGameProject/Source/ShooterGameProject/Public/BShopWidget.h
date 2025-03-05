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

	class ABBaseItem* ItemToSell = nullptr;
	
	UFUNCTION(BlueprintCallable)
	void DisplayPlayerCoin();
	UFUNCTION(BlueprintCallable)
	void AddItemsToScrollBox();

	UFUNCTION(BlueprintCallable)
	void ConfirmPurchase();
	UFUNCTION(BlueprintCallable)
	void ExecutePurchase();
	UFUNCTION(BlueprintCallable)
	void CancelPurchase();

	UFUNCTION(BlueprintCallable)
	void ShowInventory();
	UFUNCTION(BlueprintCallable)
	void ConfirmSell();
	UFUNCTION(BlueprintCallable)
	void ExecuteSell();
	UFUNCTION(BlueprintCallable)
	void CancelSell();
};
