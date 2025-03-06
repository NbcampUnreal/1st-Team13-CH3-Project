#include "KillLogWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UKillLogWidget::UpdateKillLog_Implementation(const FName& Name, UTexture2D* IconTexture)
{
	if (KillerName)
	{
		KillerName->SetText(FText::FromString(TEXT("Player")));
	}

	if (KilledName)
	{
		KilledName->SetText(FText::FromName(Name));
	}

	if (WeaponIcon)
	{
		if (IconTexture)
		{
			WeaponIcon->SetBrushFromTexture(IconTexture);
		}
	}
}

