#include "BShopRowWidget.h"
#include "BShopItemRow.h"
#include "BShopWidget.h"
#include "BBaseItem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Blueprint/WidgetTree.h"

void UBShopRowWidget::SetWidgetValues(const FBShopItemRow& ItemRow, float PriceMultiplier)
{
	RowName = FName(ItemRow.ItemName);

	if (Icon)
	{
		Icon->SetBrushFromTexture(ItemRow.ItemTexture);
	}

	if (Name)
	{
		Name->SetText(FText::FromString(ItemRow.DisplayName));
	}

	if (Price)
	{
		Price->SetText(FText::FromString(FString::Printf(TEXT("%d G"), static_cast<int32>(ItemRow.Price * PriceMultiplier))));
		SellPrice = static_cast<int32>(ItemRow.Price * PriceMultiplier);
	}

	if (Info)
	{
		Info->SetText(FText::FromString(ItemRow.Info));
	}
}

void UBShopRowWidget::NotifyClicked()
{
	if (ShopWidget)
	{
		if (ShopWidget->bIsBuying)
		{
			ShopWidget->ConfirmPurchase(RowName);
		}
		else
		{
			ShopWidget->ConfirmSell(InventoryItemName, InventoryItemRef, SellPrice);
		}
	}
}
