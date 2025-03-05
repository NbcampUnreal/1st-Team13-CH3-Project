#include "BShopWidget.h"
#include "BShopRowWidget.h"
#include "BShopItemRow.h"
#include "BPlayerState.h"
#include "BBaseItem.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Engine/DataTable.h"

void UBShopWidget::DisplayPlayerCoin()
{
	if (PlayerCoinText)
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (ABPlayerState* PlayerState = PlayerController->GetPlayerState<ABPlayerState>())
			{
				int32 PlayerCoin = PlayerState->GetCoin();
				//int32 PlayerGold = 50; // TODO: DELETE
				PlayerCoinText->SetText(FText::FromString(FString::Printf(TEXT("%d Coins"), PlayerCoin)));
			}
		}
	}
}

void UBShopWidget::AddItemsToScrollBox()
{
	if (ShopDataTable && ShopScrollBox)
	{
		static const FString ContextString(TEXT("ShopContext"));
		TArray<FBShopItemRow*> ShopDataTableRows;
		ShopDataTable->GetAllRows(ContextString, ShopDataTableRows);

		if (ShopDataTableRows.IsEmpty()) return;

		for (FBShopItemRow* Row : ShopDataTableRows)
		{
			if (Row)
			{
				if (UBShopRowWidget* ShopRowWidget = CreateWidget<UBShopRowWidget>(this, ShopRowWidgetClass))
				{
					ShopRowWidget->SetWidgetValues(*Row);
					ShopScrollBox->AddChild(ShopRowWidget);

					if (UScrollBoxSlot* ShopRowWidgetSlot = Cast<UScrollBoxSlot>(ShopRowWidget->Slot))
					{
						ShopRowWidgetSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 20.f));
					}
				}
			}
		}
	}
}

/********** BUY **********/
// Display pop-up message confirming purchase
void UBShopWidget::ConfirmPurchase()
{
	// Name

	// Price

}

// Execute purchase logic on a YES button click
void UBShopWidget::ExecutePurchase()
{
	// Get information for the clicked item from the sub-widget
	
	// Conditions
	// 1. character's got enough gold
	// 2. when added, count of the item will not exceed the limit

	// Execute
	// 1. Deduct character's gold
	// 2. Create the item object
	// 3. Add it to player's inventory
}

// Collapse pop-up message on a NO button click
void UBShopWidget::CancelPurchase()
{
	
}

/********* SELL *********/
// Display inventory to let player select the item to sell
void UBShopWidget::ShowInventory()
{
	
}

// To be called when an item is clicked in the inventory
// Need to get a reference to the item that's clicked
// Display a pop-up message confirming sell
void UBShopWidget::ConfirmSell()
{
	// Name

	// Price
	
	// Upgrade info if it's a weapon??
}

// Execute sell logic on a YES button click
void UBShopWidget::ExecuteSell()
{
	// Remove the item from the inventory
}

// Collapse pop-up message on a NO button click
void UBShopWidget::CancelSell()
{

}
