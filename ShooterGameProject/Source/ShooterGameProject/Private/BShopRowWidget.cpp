#include "BShopRowWidget.h"
#include "BShopItemRow.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"


void UBShopRowWidget::SetWidgetValues(const FBShopItemRow& ItemRow)
{
	if (Icon)
	{
		Icon->SetBrushFromTexture(ItemRow.ItemTexture);
	}

	if (Name)
	{
		Name->SetText(FText::FromString(ItemRow.DisplayName));
	}

	if (Price)
	{
		Price->SetText(FText::FromString(FString::Printf(TEXT("%d G"),ItemRow.Price)));
	}

	if (Info)
	{
		Info->SetText(FText::FromString(ItemRow.Info));
	}
}