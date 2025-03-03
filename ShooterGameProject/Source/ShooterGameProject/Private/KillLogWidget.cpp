#include "KillLogWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UKillLogWidget::UpdateKillLog_Implementation(const FName& Name, const FString& WeaponType)
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
		if (WeaponType == "Pistol")
		{
			WeaponIconTexture = LoadObject<UTexture2D>(this, TEXT("/Game/UI/Textures/T_Pistol_Icon.T_Pistol_Icon"));
		}
		else if (WeaponType == "Rifle")
		{
			WeaponIconTexture = LoadObject<UTexture2D>(this, TEXT("/Game/UI/Textures/T_Rifle_Icon.T_Rifle_Icon"));
		}
		else if (WeaponType == "Shotgun")
		{
			WeaponIconTexture = LoadObject<UTexture2D>(this, TEXT("/Game/UI/Textures/T_Shotgun_Icon.T_Shotgun_Icon"));
		}
		else if (WeaponType == "Melee")
		{
			WeaponIconTexture = LoadObject<UTexture2D>(this, TEXT("/Game/UI/Textures/T_Melee_Icon.T_Melee_Icon"));
		}
		else if (WeaponType == "Grenade")
		{
			WeaponIconTexture = LoadObject<UTexture2D>(this, TEXT("/Game/UI/Textures/T_Grenade_Icon.T_Grenade_Icon"));
		}

		if (WeaponIconTexture)
		{
			WeaponIcon->SetBrushFromTexture(WeaponIconTexture);
		}
	}
}

void UKillLogWidget::DeleteTexture()
{
	if (WeaponIconTexture)
	{
		delete WeaponIconTexture;
		WeaponIconTexture = nullptr;
	}
}

