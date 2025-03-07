#include "WeaponAmmoWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UWeaponAmmoWidget::UpdateWeapon(UTexture2D* WeaponTexture)
{
	if (UImage* WeaponImage = Cast<UImage>(GetWidgetFromName(TEXT("WeaponImage"))))
	{
		if (WeaponTexture)
		{
			WeaponImage->SetBrushFromTexture(WeaponTexture);
		}
	}
}

void UWeaponAmmoWidget::UpdateLoadedAmmo(const int32& LoadedAmmo)
{
	if (UTextBlock* LoadedAmmoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("LoadedAmmoText"))))
	{
		if (LoadedAmmo == -1)
		{
			LoadedAmmoText->SetText(FText::FromString(TEXT("∞")));
		}
		else
		{
			LoadedAmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d"), LoadedAmmo)));
		}
		
	}
}

void UWeaponAmmoWidget::UpdateInventoryAmmo(const int32& InventoryAmmo)
{
	if (UTextBlock* InventoryAmmoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("InventoryAmmoText"))))
	{
		if (InventoryAmmo == -1)
		{
			InventoryAmmoText->SetText(FText::FromString(TEXT("∞")));
		}
		else
		{
			InventoryAmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d"), InventoryAmmo)));
		}
	}
}