#include "BShopWidget.h"
#include "BShopRowWidget.h"
#include "BShopItemRow.h"
#include "BCharacter.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Engine/DataTable.h"

void UBShopWidget::DisplayPlayerGold()
{
	if (PlayerGoldText)
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (ABCharacter* BCharacter = Cast<ABCharacter>(PlayerController->GetCharacter()))
			{
				// int32 PlayerGold = BCharacter->GetGold(); 
				int32 PlayerGold = 50; // TODO: DELETE
				PlayerGoldText->SetText(FText::FromString(FString::Printf(TEXT("%d G"), PlayerGold)));
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

void UBShopWidget::ConfirmPurchase()
{
	// Display a pop-up message confirming purchase
}

// Execute purchase on a Yes button click
void UBShopWidget::ExecutePurchase()
{
	// Check character's gold amount, deduct the price from character's gold, and add the item to the inventory
}

// Cancel purchase on a No button click
void UBShopWidget::CancelPurchase()
{
	// Remove the pop-up message
}
