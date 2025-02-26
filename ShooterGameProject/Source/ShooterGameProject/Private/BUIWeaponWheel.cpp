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
			ABBaseItem* EquippedWeapon = BCharacter->EquippedWeapons[(int32)EWeaponSlot::Primary];

			// Equip Pistol
		// TODO: Equip the pistol that's actually in the inventory
		// QUESTION: If there are multipel pistols,,, in the inventory,,, do i need to iterate through the whole inventory to find the best one?
		// 임시로 피스톨 만들어 장착 시켜봄
			if (HoveredItemName == "Pistol")
			{
				// Return if it's not already equipped
				if (EquippedWeapon && EquippedWeapon->IsA<ABPistol>()) return;

				// THIS IS TEMPORARY!!!!!!
				ABPistol* TempPistol = NewObject<ABPistol>(BCharacter);
				BCharacter->EquipWeapon(TempPistol);
				return;
			}

			// Equip Rifle
			if (HoveredItemName == "Rifle")
			{
				// Return if rifle is already equipped
				if (EquippedWeapon && EquippedWeapon->IsA<ABRifle>()) return;

				ABRifle* TempRifle = NewObject<ABRifle>(BCharacter);
				BCharacter->EquipWeapon(TempRifle);	
				return;
			}

			
			// Equip Shotgun
			if (HoveredItemName == "Shotgun")
			{
				// Return if Shotgun is already equipped
				if (EquippedWeapon && EquippedWeapon->IsA<ABShotgun>()) return;

				ABShotgun* TempShotgun = NewObject<ABShotgun>(BCharacter);
				BCharacter->EquipWeapon(TempShotgun);
				return;
			}
			

			// Equip Shotgun
			if (HoveredItemName == "Melee")
			{
				// Return if Melee is already equipped
				if (EquippedWeapon && EquippedWeapon->IsA<ABMeleeWeapon>()) return;

				ABMeleeWeapon* TempMeleeWeapon = NewObject<ABMeleeWeapon>(BCharacter);
				BCharacter->EquipWeapon(TempMeleeWeapon);
				return;
			}

			// Equip Grenade
			if (HoveredItemName == "Grenade")
			{
				// Return if Grenade is already equipped
				if (EquippedWeapon && EquippedWeapon->IsA<ABGrenadeWeapon>()) return;

				ABGrenadeWeapon* TempGrenade = NewObject<ABGrenadeWeapon>(BCharacter);
				BCharacter->EquipWeapon(TempGrenade);
				return;
			}
		}
	}
}
