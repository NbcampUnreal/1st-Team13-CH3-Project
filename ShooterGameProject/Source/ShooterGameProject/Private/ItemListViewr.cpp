
#include "ItemListViewr.h"
#include "Components/VerticalBox.h"
#include "Components/TreeView.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Components/Border.h"
#include "TreeViewItemObject.h"
#include "InventoryDragDropOperation.h"
void UItemListViewr::NativeConstruct()
{
	Super::NativeConstruct();

	VerticalBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("VB"));
	ListTitle = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ListTitle"));
	Overlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Overlay"));
	Border = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Border"));
	ItemListView = WidgetTree->ConstructWidget<UTreeView>(UTreeView::StaticClass(), TEXT("ItemListView"));

	WidgetTree->RootWidget = VerticalBox;

	VerticalBox->AddChild(ListTitle);
	VerticalBox->AddChild(Overlay);
	Overlay->AddChild(Border);
	Overlay->AddChild(ItemListView);

	if (Border)
	{
		Border->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.5f));
	}
}

// Drop이 되었을때 실행되는 함수.
bool UItemListViewr::NativeOnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent, UDragDropOperation* Operation)
{
	Super::NativeOnDrop(MyGeometry, DragDropEvent, Operation);
	WidgetOperation = Cast<UInventoryDragDropOperation>(Operation);
	if (WidgetOperation != nullptr)
	{
		const FVector2D DragOffset = MyGeometry.AbsoluteToLocal(DragDropEvent.GetScreenSpacePosition());
		const FVector2D DragOffsetResult = DragOffset - WidgetOperation->DragOffset;

		WidgetOperation->WidgetReference->AddToViewport();
		WidgetOperation->WidgetReference->SetVisibility(ESlateVisibility::Visible);
		WidgetOperation->WidgetReference->SetPositionInViewport(DragOffsetResult, false);
		
		AddItem(ItemListView);
		
		return true;
	}

	return false;
}

void UItemListViewr::AddItem(UTreeView* TreeView)
{
	if (WidgetOperation)
	{
		TreeView->AddItem(WidgetOperation->WidgetReference);
	}
}

void UItemListViewr::RemoveItem(UTreeView* TreeView)
{
	if (WidgetOperation)
	{
		TreeView->RemoveItem(WidgetOperation->WidgetReference);
	}
}

