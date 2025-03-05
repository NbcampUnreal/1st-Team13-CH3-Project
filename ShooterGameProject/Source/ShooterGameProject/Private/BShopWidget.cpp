#include "BShopWidget.h"
#include "BShopRowWidget.h"
#include "BShopItemRow.h"
#include "BPlayerState.h"
#include "BBaseItem.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/Border.h"
#include "Components/Button.h"
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
				PlayerCoinText->SetText(FText::FromString(FString::Printf(TEXT("%d G"), PlayerCoin)));
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
					ShopRowWidget->SetWidgetValues(*Row, 1.f);
					ShopRowWidget->ShopWidget = this;
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
void UBShopWidget::ConfirmPurchase(const FName& RowName)
{
	if (ShopDataTable)
	{
		PurchaseItemRow = ShopDataTable->FindRow<FBShopItemRow>(RowName, TEXT("ShopContext"));
		if (PurchaseItemRow)
		{
			// Display name & price in the message box
			FString StringToSet = TEXT("Purchase ") + PurchaseItemRow->DisplayName + FString::Printf(TEXT(" for %d Gold?"), PurchaseItemRow->Price);
			MessageText->SetText(FText::FromString(StringToSet));
			MessageBorder->SetVisibility(ESlateVisibility::Visible);
		}
	}

	ShopScrollBox->SetVisibility(ESlateVisibility::HitTestInvisible);
}

// Execute purchase logic on a YES button click
void UBShopWidget::ExecutePurchase()
{
	if (PurchaseItemRow)
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (ABPlayerState* PlayerState = PlayerController->GetPlayerState<ABPlayerState>())
			{
				// if character's got enough gold
				if (PlayerState->GetCoin() >= PurchaseItemRow->Price)
				{
					// Deduct character's gold
					PlayerState->AddCoin(PurchaseItemRow->Price * -1);

					// Add item to player's inventory
					FItemData PurchaseItemData;
					PurchaseItemData.ItemClass = PurchaseItemRow->ItemClass;
					PurchaseItemData.ItemCount = 1;
					PurchaseItemData.ItemIcon = PurchaseItemRow->ItemTexture;
					PurchaseItemData.ItemName = PurchaseItemRow->ItemName;
					FVector InvisibleLocation = FVector::ZeroVector;
					InvisibleLocation.Z = -1000.f;
					ABBaseItem* SpawnedItem = GetWorld()->SpawnActor<ABBaseItem>(PurchaseItemData.ItemClass, InvisibleLocation, FRotator::ZeroRotator);
					PurchaseItemData.ItemRef = SpawnedItem;
					PlayerState->InventoryAddItem(PurchaseItemData);
					MessageText->SetText(FText::FromString(TEXT("Purchase was successful!")));
				}
				else
				{
					MessageText->SetText(FText::FromString(TEXT("Uh oh! Not enough gold in possession.")));
				}
				YesButton->SetVisibility(ESlateVisibility::Collapsed);
				NoButtonText->SetText(FText::FromString(TEXT("RETURN")));
			}
		}
	}


}

// Collapse pop-up message on a NO / RETURN button click
void UBShopWidget::CloseMessage()
{
	// Reset Purchase/Sell related variables
	PurchaseItemRow = nullptr;
	SellItemName = FName();
	SellItemRef = nullptr;
	SellItemPrice = 0.f;

	// Reset widgets that changed during purchase
	YesButton->SetVisibility(ESlateVisibility::Visible);
	NoButtonText->SetText(FText::FromString(TEXT("NO")));
	MessageBorder->SetVisibility(ESlateVisibility::Collapsed);
	SellScrollBox->SetVisibility(ESlateVisibility::Visible);
	ShopScrollBox->SetVisibility(ESlateVisibility::Visible);
}

/********* SELL *********/
// Display inventory items to let player select the item to sell
void UBShopWidget::AddInventoryItemsToScrollBox()
{
	if (ShopDataTable && SellScrollBox)
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (ABPlayerState* PlayerState = PlayerController->GetPlayerState<ABPlayerState>())
			{
				TArray<FItemData> InventoryItems = PlayerState->GetAllInventoryItem();
				for (const FItemData& Item : InventoryItems)
				{
					if (FBShopItemRow* Row = ShopDataTable->FindRow<FBShopItemRow>(Item.ItemName, TEXT("ShopContext")))
					{
						if (UBShopRowWidget* ShopRowWidget = CreateWidget<UBShopRowWidget>(this, ShopRowWidgetClass))
						{
							ShopRowWidget->SetWidgetValues(*Row, SellPriceMultiplier);
							ShopRowWidget->ShopWidget = this;
							ShopRowWidget->InventoryItemName = Item.ItemName;
							ShopRowWidget->InventoryItemRef = Item.ItemRef;
							SellScrollBox->AddChild(ShopRowWidget);

							if (UScrollBoxSlot* ShopRowWidgetSlot = Cast<UScrollBoxSlot>(ShopRowWidget->Slot))
							{
								ShopRowWidgetSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 20.f));
							}
						}
					}
					else
					{
						GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, TEXT("Sell, FindRow failed"));
					}
				}
			}
		}
	}
}

// To be called when an item is clicked in the inventory
// Need to get a reference to the item that's clicked
// Display a pop-up message confirming sell
void UBShopWidget::ConfirmSell(const FName& ItemName, ABBaseItem* ItemRef, int32 SellPrice)
{
	SellItemName = FName(ItemName);
	SellItemRef = ItemRef;
	SellItemPrice = SellPrice;

	FString StringToSet = TEXT("Sell ") + ItemName.ToString() + FString::Printf(TEXT(" for %d Gold?"), SellPrice);
	if (MessageText)
	{
		MessageText->SetText(FText::FromString(StringToSet));
	}
	
	if (MessageBorder)
	{
		MessageBorder->SetVisibility(ESlateVisibility::Visible);
	}

	SellScrollBox->SetVisibility(ESlateVisibility::HitTestInvisible);
}

// Execute sell logic on a YES button click
void UBShopWidget::ExecuteSell()
{
	// Remove the item from the inventory
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ABPlayerState* PlayerState = PlayerController->GetPlayerState<ABPlayerState>())
		{
			TArray<FItemData> InventoryItems = PlayerState->GetInventoryTypeItem(SellItemName);
			for (const FItemData& Item : InventoryItems)
			{
				if (SellItemRef == Item.ItemRef)
				{
					PlayerState->AddCoin(SellItemPrice);
					PlayerState->InventoryRemoveItem(Item);
					MessageText->SetText(FText::FromString(TEXT("Sale was successful!")));
					break;
				}
			}
			YesButton->SetVisibility(ESlateVisibility::Collapsed);
			NoButtonText->SetText(FText::FromString(TEXT("RETURN")));
		}
	}
}
