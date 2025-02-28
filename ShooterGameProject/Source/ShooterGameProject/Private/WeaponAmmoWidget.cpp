#include "WeaponAmmoWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UWeaponAmmoWidget::UWeaponAmmoWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
	/**** Get and assign textures ****/
	ConstructorHelpers::FObjectFinder<UTexture2D> PistolIconFinder(TEXT("/Game/UI/Textures/T_Pistol_Icon.T_Pistol_Icon"));
	if (PistolIconFinder.Succeeded())
	{
		PistolIconTexture = PistolIconFinder.Object;
	}

	ConstructorHelpers::FObjectFinder<UTexture2D> RifleIconFinder(TEXT("/Game/UI/Textures/T_Rifle_Icon.T_Rifle_Icon"));
	if (RifleIconFinder.Succeeded())
	{
		RifleIconTexture = RifleIconFinder.Object;
	}

	ConstructorHelpers::FObjectFinder<UTexture2D> MeleeIconFinder(TEXT("/Game/UI/Textures/T_Melee_Icon.T_Melee_Icon"));
	if (MeleeIconFinder.Succeeded())
	{
		MeleeIconTexture = MeleeIconFinder.Object;
	}
}

void UWeaponAmmoWidget::UpdateWeapon(const FName& WeaponType)
{
	UTexture2D* WeaponTexture = nullptr;
	if (WeaponType == "Pistol" && IsValid(PistolIconTexture))
	{
		WeaponTexture = PistolIconTexture;
	}
	else if (WeaponType == "Rifle" && IsValid(RifleIconTexture))
	{
		WeaponTexture = RifleIconTexture;
	}
	else if (WeaponType == "Melee" && IsValid(MeleeIconTexture))
	{
		WeaponTexture = MeleeIconTexture;
	}


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
		LoadedAmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d"), LoadedAmmo)));
	}
}

void UWeaponAmmoWidget::UpdateInventoryAmmo(const int32& InventoryAmmo)
{
	if (UTextBlock* InventoryAmmoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("InventoryAmmoText"))))
	{
		InventoryAmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d"), InventoryAmmo)));
	}
}