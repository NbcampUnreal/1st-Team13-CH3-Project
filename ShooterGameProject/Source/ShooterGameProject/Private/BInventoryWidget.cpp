#include "BInventoryWidget.h"
#include "ItemListViewr.h"
#include "BBaseItem.h"
#include "BPlayerState.h"
void UBInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (NearList)
	{
		NearList->SetTreeType(ETreeViewType::NearItem);
		NearList->SetTitle(FText::FromString(TEXT("주변 아이템")));
	}
	if (InventoryList)
	{
		InventoryList->SetTreeType(ETreeViewType::Inventory);
		InventoryList->SetTitle(FText::FromString(TEXT("인벤토리")));
	}
}

void UBInventoryWidget::SendItemData(const TArray<class ABBaseItem*>& ActicvatedItem, class ABPlayerState* PlayerState)
{
	if (NearList)
	{
		NearList->SetPlayerState(PlayerState);
	}
	if (InventoryList)
	{
		InventoryList->SetPlayerState(PlayerState);
	}
	if (!ActicvatedItem.IsEmpty())
	{
		AddNearList(ActicvatedItem);
	}
	if (PlayerState)
	{
		AddInventoryList(PlayerState);
	}
}

UItemListViewr* UBInventoryWidget::GetNearListTreeView() const
{
	if (NearList)
		return NearList;
	else
		return nullptr;
}

UItemListViewr* UBInventoryWidget::GetInventoryListTreeView() const
{
	if (InventoryList)
		return InventoryList;
	else
		return nullptr;
}

void UBInventoryWidget::AddNearList(const TArray<class ABBaseItem*>& NearItemList)
{
	if (UItemListViewr* Near = GetNearListTreeView())
	{
		if (TreeItemWidgetClass.IsValid())
		{
			if (UClass* LoadedClass = TreeItemWidgetClass.Get())
			{
				for (const auto& BaseItem : NearItemList)
				{
					if (TreeItemWidgetClass)
					{
						UTreeItem* Item = CreateWidget<UTreeItem>(Near->GetTreeView(), LoadedClass);
						Item->SetItemData(BaseItem->GetItemData());
						Item->SetOwnerTreeView(Near->GetTreeView());
						Item->RefreshData();
						Near->AddItem(Near->GetTreeView(), Item);
					}
				}
			}
		}
	}
}

void UBInventoryWidget::AddInventoryList(class ABPlayerState* PlayerState)
{
	if (UItemListViewr* InventoryTree = GetInventoryListTreeView())
	{
		if (TreeItemWidgetClass.IsValid())
		{
			if (UClass* LoadedClass = TreeItemWidgetClass.Get())
			{
				for (const auto& ItemRef : PlayerState->GetAllInventoryItem())
				{
					if (TreeItemWidgetClass)
					{
						UTreeItem* Item = CreateWidget<UTreeItem>(InventoryTree->GetTreeView(), LoadedClass);
						Item->SetItemData(ItemRef);
						Item->SetOwnerTreeView(InventoryTree->GetTreeView());
						Item->RefreshData();
						InventoryTree->AddItem(InventoryTree->GetTreeView(), Item);
					}
				}
			}
		}
	}
}