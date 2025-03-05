#include "BUIWeaponWheel.h"
#include "BCharacter.h"
#include "BBaseItem.h"
#include "BFirstAidKit.h"
#include "BPistol.h"
#include "BRifle.h"
#include "BShotgun.h"
#include "BMeleeWeapon.h"
#include "BGrenadeWeapon.h"


void UBUIWeaponWheel::ExitWeaponWheel_Implementation()
{
	if (HoveredItemName == NAME_None) return;

	if (HoveredItemName == "FirstAidKit")
	{
		// TODO: Use First Aid Kit Item 
	}
	
	// Get currently equipped weapon
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ABCharacter* BCharacter = Cast<ABCharacter>(PlayerController->GetCharacter()))
		{
			// Equip Pistol
			if (HoveredItemName == "Pistol")
			{
				BCharacter->EquipPistol();
				return;
			}

			// Equip Rifle
			if (HoveredItemName == "Rifle")
			{
				BCharacter->EquipRifle();
				return;
			}

			
			// Equip Shotgun
			if (HoveredItemName == "Shotgun")
			{
				BCharacter->EquipShotgun();
				return;
			}
			

			// Equip Shotgun
			if (HoveredItemName == "Melee")
			{
				BCharacter->EquipMelee();
				return;
			}

			// Equip Grenade
			if (HoveredItemName == "Grenade")
			{
				//BCharacter->EquipGrenade();
				return;
			}
		}
	}
}
