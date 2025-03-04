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
	// TODO :Player의 인벤토리를 가져와야함
}

void UBInventoryWidget::AddNearTestList()
{
	if (UItemListViewr* Near = GetNearListTreeView())
	{
		if (TreeItemWidgetClass.IsValid())
		{
			if (UClass* LoadedClass = TreeItemWidgetClass.Get())
			{
				for (int i = 0; i < 5; ++i)
				{
					if (TreeItemWidgetClass)
					{
						UTreeItem* Item = CreateWidget<UTreeItem>(Near->GetTreeView(), LoadedClass);
						Item->SetOwnerTreeView(Near->GetTreeView());
						Item->SetItemName(TEXT("Test"));
						Near->AddItem(Near->GetTreeView(), Item);
					}
				}
			}
		}
	}
}
