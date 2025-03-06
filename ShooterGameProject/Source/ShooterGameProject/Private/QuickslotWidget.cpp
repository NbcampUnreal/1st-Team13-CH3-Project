#include "QuickslotWidget.h"
#include "Components/TextBlock.h"

void UQuickslotWidget::UpdateQuickslot(const FName& ItemName, const int32& Count)
{
	// Based on ItemName, change the count of that item in quick slot
	FString WidgetNameString = ItemName.ToString() + TEXT("Count");
	FName WidgetName(*WidgetNameString);	
	UE_LOG(LogTemp, Warning, TEXT("Count : %d"), Count);
	if (UTextBlock* QuickslotCount = Cast<UTextBlock>(GetWidgetFromName(WidgetName)))
	{
		QuickslotCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), Count)));
	}
}