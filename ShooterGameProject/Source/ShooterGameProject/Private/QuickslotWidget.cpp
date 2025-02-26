#include "QuickslotWidget.h"
#include "Components/TextBlock.h"

void UQuickslotWidget::UpdateQuickslot(const FName& ItemName, const int32& Count)
{
	// TODO: Based on ItemType, change the count of that item in quick slot
	FName WidgetName = "";
	if (ItemName == "FirstAidKit")
	{
		WidgetName = "QuickslotCount_0";
	}
	else if (ItemName == "Grenade")
	{
		WidgetName = "QuickslotCount_1";
	}

	if (UTextBlock* QuickslotText = Cast<UTextBlock>(GetWidgetFromName(WidgetName)))
	{
		QuickslotText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Count)));
	}
}