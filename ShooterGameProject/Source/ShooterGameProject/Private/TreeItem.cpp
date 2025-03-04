#include "TreeItem.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "TreeViewItemObject.h"
#include "InventoryDragDropOperation.h"
void UTreeItem::NativeConstruct()
{
	Super::NativeConstruct();	
}

void UTreeItem::SetItemName(const FName& Name)
{
	ItemData.ItemName = Name;
}

void UTreeItem::SetItemIcon(UTexture2D* Icon)
{
	ItemData.ItemIcon = Icon;
}

void UTreeItem::SetItemType(UClass* Class)
{
	ItemData.ItemClass = Class;
}

FName UTreeItem::GetItemName() const
{
	return ItemData.ItemName;
}

UTexture2D* UTreeItem::GetItemIcon() const
{
	return ItemData.ItemIcon;
}

UClass* UTreeItem::GetItemClass() const
{
	return ItemData.ItemClass;
}

//	���콺 Ŭ���� ������ �̺�Ʈ
FReply UTreeItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	/*FEventReply ReplyResult =
	UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	return ReplyResult.NativeReply;*/

	return CustomDetectDrag(InMouseEvent, this, EKeys::LeftMouseButton);
}

// �巡�װ� �����Ǹ�, �ش� ���콺 ��ġ�� Operation�� ���� �Ѱ��ش�.
void UTreeItem::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UInventoryDragDropOperation* Oper = NewObject<UInventoryDragDropOperation>();
	this->SetVisibility(ESlateVisibility::HitTestInvisible);

	Oper->WidgetReference = this;
	Oper->DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	Oper->DefaultDragVisual = this;
	Oper->Pivot = EDragPivot::MouseDown;

	OutOperation = Oper;
}

void UTreeItem::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	RemoveFromParent();
}

// Ʈ���信 ������ �ֱ� ���� �Լ�.
void UTreeItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (UTreeItem* Item = Cast<UTreeItem>(ListItemObject))
	{
		ItemData = Item->ItemData;

		if (ItemData.ItemName.IsValid())
		{
			if (ItemName)
			{
				ItemName->SetText(FText::FromName(ItemData.ItemName));
			}
		}
		if (ItemData.ItemIcon)
		{
			if (ItemIcon)
			{
				ItemIcon->SetBrushFromTexture(ItemData.ItemIcon,true);
			}
		}
	}
}

FReply UTreeItem::CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey)
{
	if (InMouseEvent.GetEffectingButton() == DragKey)
	{
		FEventReply Reply;
		Reply.NativeReply = FReply::Handled();

		if (WidgetDetectingDrag)
		{
			TSharedPtr<SWidget> SlateWidgetDetectingDrag = WidgetDetectingDrag->GetCachedWidget();
			if (SlateWidgetDetectingDrag.IsValid())
			{
				Reply.NativeReply = Reply.NativeReply.DetectDrag(SlateWidgetDetectingDrag.ToSharedRef(), DragKey);
				return Reply.NativeReply;
			}
		}
	}

	return FReply::Unhandled();
}
