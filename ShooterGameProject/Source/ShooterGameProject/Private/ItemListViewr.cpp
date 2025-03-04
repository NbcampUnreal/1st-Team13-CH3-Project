
#include "ItemListViewr.h"
#include "Components/VerticalBox.h"
#include "Components/TreeView.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Components/Border.h"
#include "TreeViewItemObject.h"
#include "InventoryDragDropOperation.h"
#include "TreeItem.h"
#include "BBaseItem.h"
void UItemListViewr::NativeConstruct()
{
	Super::NativeConstruct();
}

void UItemListViewr::SetTitle(const FText& Text)
{
	ListTitle->SetText(Text);
}

void UItemListViewr::SetTreeType(const ETreeViewType& Type)
{
	TreeType = Type;
}

// Drop이 되었을때 실행되는 함수.
bool UItemListViewr::NativeOnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent, UDragDropOperation* Operation)
{
	Super::NativeOnDrop(MyGeometry, DragDropEvent, Operation);

	WidgetOperation = Cast<UInventoryDragDropOperation>(Operation);
	if (WidgetOperation != nullptr)
	{
		if (UTreeItem* TreeItem = Cast<UTreeItem>(Operation->Payload))
		{
			//const FVector2D DragOffset = MyGeometry.AbsoluteToLocal(DragDropEvent.GetScreenSpacePosition());
			//const FVector2D DragOffsetResult = DragOffset - WidgetOperation->DragOffset;

			//WidgetOperation->WidgetReference->AddToViewport();
			//WidgetOperation->WidgetReference->SetVisibility(ESlateVisibility::Visible);
			//WidgetOperation->WidgetReference->SetPositionInViewport(DragOffsetResult, false);

			ItemListView->AddItem(TreeItem);
			TreeItem->SetOwnerTreeView(ItemListView);

			switch (TreeType)
			{
			case ETreeViewType::NearItem:
				DropItem(TreeItem->GetItemClass());
				break;
			case ETreeViewType::Inventory:
				//DestroyItem(TreeItem->GetItemData)
				break;
			default:
				break;
			}
		}
		return true;
	}

	return false;
}

void UItemListViewr::AddItem(UTreeView* TreeView, UTreeItem* TreeItem)
{
	TreeView->AddItem(TreeItem);
	TreeItem->SetOwnerTreeView(TreeView);
}

void UItemListViewr::RemoveItem(UTreeView* TreeView, UTreeItem* TreeItem)
{
	if (WidgetOperation)
	{
		TreeView->RemoveItem(TreeItem);
	}
}

void UItemListViewr::DestroyItem(ABBaseItem* Item)
{
	Item->DestroyItem();
}

void UItemListViewr::DropItem(UClass* Class)
{
	// 플레이어 위치를 알아야함
	// SpawnActor를 통해 플레이어 위치에 스폰되도록 함
}

