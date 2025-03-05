
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
#include "BPlayerState.h"
#include "BCharacter.h"
#include "BPlayerController.h"
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

void UItemListViewr::SetPlayerState(ABPlayerState* State)
{
	PlayerState = State;
}

ABPlayerState* UItemListViewr::GetPlayerState() const
{
	return PlayerState;
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

			
			

			switch (TreeType)
			{
			case ETreeViewType::NearItem:
				DropItem(TreeItem);
				RemoveInventory(TreeItem->GetItemData());
				AddItem(ItemListView, TreeItem);
				break;
			case ETreeViewType::Inventory:
				InputInventory(TreeItem->GetItemData());
				AddItem(ItemListView, TreeItem);
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
	TreeView->RemoveItem(TreeItem);
}

void UItemListViewr::DestroyItem(ABBaseItem* Item)
{
	Item->DestroyItem();
}

ABBaseItem* UItemListViewr::SpawnItem(const FItemData& ItemInfo)
{
	// 플레이어 위치를 알아야함
	if (PlayerState)
	{
		if (ABPlayerController* Control = Cast<ABPlayerController>(PlayerState->GetOwner()))
		{
			if (ABCharacter* Chr = Cast<ABCharacter>(Control->GetCharacter()))
			{
				FVector PlayerLocation = Chr->GetActorLocation();
				// SpawnActor를 통해 플레이어 위치에 스폰되도록 함
				if (UWorld* World = GetWorld())
				{
					auto ItemBase = World->SpawnActor<ABBaseItem>(ItemInfo.ItemClass, PlayerLocation, FRotator::ZeroRotator);
					UE_LOG(LogTemp, Warning, TEXT("WorldItem Spawn"));
					return ItemBase;
				}
			}
		}
	}

	return nullptr;
}

void UItemListViewr::DropItem(UTreeItem* TreeItem)
{
	ABBaseItem* Spawn = SpawnItem(TreeItem->GetItemData());
	TreeItem->SetItemData(Spawn->GetItemData());
}

void UItemListViewr::InputInventory(const FItemData& ItemInfo)
{
	PlayerState->InventoryAddItem(ItemInfo);
	
	if (IsValid(ItemInfo.ItemRef))
	{
		DestroyItem(ItemInfo.ItemRef);		
		UE_LOG(LogTemp, Warning, TEXT("WorldItem Dstroy"));
	}
}

void UItemListViewr::RemoveInventory(const FItemData& ItemInfo)
{
	PlayerState->InventoryRemoveItem(ItemInfo);
}

