#include "BUIWeaponWheel.h"
#include "BCharacter.h"
#include "BPlayerState.h"
#include "BGameInstance.h"
#include "BUIManager.h"
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

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ABCharacter* BCharacter = Cast<ABCharacter>(PlayerController->GetCharacter()))
		{
			if (ABPlayerState* PlayerState = BCharacter->GetPlayerState<ABPlayerState>())
			{
				if (HoveredItemName == "FirstAidKit")
				{
					TArray<FItemData> InventoryItems;
					InventoryItems = PlayerState->GetInventoryTypeItem(HoveredItemName);
					if (InventoryItems.IsEmpty())
					{
						// Item doesn't exist in the inventory - notification
						if (UBGameInstance* GameInstance = GetGameInstance<UBGameInstance>())
						{
							if (UBUIManager* UIManager = GameInstance->GetUIManagerInstance())
							{
								FString Message = TEXT("Couldn't find ") + HoveredItemName.ToString() + TEXT(" in the inventory.");
								UIManager->DisplayNotification("Cannot find item!", Message);
							}
						}
						return;
					}
					PlayerState->UseItem(HoveredItemName);
					return;
				}
			}

			

			// Equip Pistol
			if (HoveredItemName == "Pistol")
			{
				if (BCharacter->EquippedWeapons[(int32)EWeaponSlot::Pistol])
				{
					BCharacter->EquipPistol();
				}
				else
				{
					// Item doesn't exist in the inventory - notification
					if (UBGameInstance* GameInstance = GetGameInstance<UBGameInstance>())
					{
						if (UBUIManager* UIManager = GameInstance->GetUIManagerInstance())
						{
							FString Message = TEXT("Couldn't find ") + HoveredItemName.ToString() + TEXT(" in the inventory.");
							UIManager->DisplayNotification("Cannot find item!", Message);
						}
					}
				}
				
				return;
			}

			// Equip Rifle
			if (HoveredItemName == "Rifle")
			{
				if (BCharacter->EquippedWeapons[(int32)EWeaponSlot::Rifle])
				{
					BCharacter->EquipRifle();
				}
				else
				{
					// Item doesn't exist in the inventory - notification
					if (UBGameInstance* GameInstance = GetGameInstance<UBGameInstance>())
					{
						if (UBUIManager* UIManager = GameInstance->GetUIManagerInstance())
						{
							FString Message = TEXT("Couldn't find ") + HoveredItemName.ToString() + TEXT(" in the inventory.");
							UIManager->DisplayNotification("Cannot find item!", Message);
						}
					}
				}
				return;
			}

			
			// Equip Shotgun
			if (HoveredItemName == "Shotgun")
			{
				if (BCharacter->EquippedWeapons[(int32)EWeaponSlot::ShotGun])
				{
					BCharacter->EquipShotgun();
				}
				else
				{
					// Item doesn't exist in the inventory - notification
					if (UBGameInstance* GameInstance = GetGameInstance<UBGameInstance>())
					{
						if (UBUIManager* UIManager = GameInstance->GetUIManagerInstance())
						{
							FString Message = TEXT("Couldn't find ") + HoveredItemName.ToString() + TEXT(" in the inventory.");
							UIManager->DisplayNotification("Cannot find item!", Message);
						}
					}
				}
				return;
			}
			

			// Equip Shotgun
			if (HoveredItemName == "Melee")
			{
				if (BCharacter->EquippedWeapons[(int32)EWeaponSlot::Melee])
				{
					BCharacter->EquipMelee();
				}
				else
				{
					// Item doesn't exist in the inventory - notification
					if (UBGameInstance* GameInstance = GetGameInstance<UBGameInstance>())
					{
						if (UBUIManager* UIManager = GameInstance->GetUIManagerInstance())
						{
							FString Message = TEXT("Couldn't find ") + HoveredItemName.ToString() + TEXT(" in the inventory.");
							UIManager->DisplayNotification("Cannot find item!", Message);
						}
					}
				}
				return;
			}

			// Equip Grenade
			if (HoveredItemName == "Grenade")
			{
				if (BCharacter->EquippedWeapons[(int32)EWeaponSlot::Throwable])
				{
					BCharacter->EquipGrenade();
				}
				else
				{
					// Item doesn't exist in the inventory - notification
					if (UBGameInstance* GameInstance = GetGameInstance<UBGameInstance>())
					{
						if (UBUIManager* UIManager = GameInstance->GetUIManagerInstance())
						{
							FString Message = TEXT("Couldn't find ") + HoveredItemName.ToString() + TEXT(" in the inventory.");
							UIManager->DisplayNotification("Cannot find item!", Message);
						}
					}
				}
				return;
			}
		}
	}
}
