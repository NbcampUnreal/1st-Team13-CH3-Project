#include "WeaponAmmoWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

//UWeaponAmmoWidget::UWeaponAmmoWidget(const FObjectInitializer& ObjectInitializer)
//	: UUserWidget(ObjectInitializer)
//{
	/**** Get and assign textures ****/
	//ConstructorHelpers::FObjectFinder<UTexture2D> PistolIconFinder(TEXT("/Game/UI/Textures/T_Pistol_Icon.T_Pistol_Icon"));
	//if (PistolIconFinder.Succeeded())
	//{
	//	PistolIconTexture = PistolIconFinder.Object;
	//}

	//ConstructorHelpers::FObjectFinder<UTexture2D> RifleIconFinder(TEXT("/Game/UI/Textures/T_Rifle_Icon.T_Rifle_Icon"));
	//if (RifleIconFinder.Succeeded())
	//{
	//	RifleIconTexture = RifleIconFinder.Object;
	//}

	//ConstructorHelpers::FObjectFinder<UTexture2D> ShotgunIconFinder(TEXT("/Game/UI/Textures/T_Shotgun_Icon.T_Shotgun_Icon"));
	//if (ShotgunIconFinder.Succeeded())
	//{
	//	ShotgunIconTexture = ShotgunIconFinder.Object;
	//}

	//ConstructorHelpers::FObjectFinder<UTexture2D> MeleeIconFinder(TEXT("/Game/UI/Textures/T_Melee_Icon.T_Melee_Icon"));
	//if (MeleeIconFinder.Succeeded())
	//{
	//	MeleeIconTexture = MeleeIconFinder.Object;
	//}

	//ConstructorHelpers::FObjectFinder<UTexture2D> GrenadeIconFinder(TEXT("/Game/UI/Textures/T_Grenade_Icon.T_Grenade_Icon"));
	//if (GrenadeIconFinder.Succeeded())
	//{
	//	GrenadeIconTexture = GrenadeIconFinder.Object;
	//}
//}

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
			LoadedAmmoText->SetText(FText::FromString(TEXT("")));
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
			InventoryAmmoText->SetText(FText::FromString(TEXT("")));
		}
		else
		{
			InventoryAmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d"), InventoryAmmo)));
		}
	}
}