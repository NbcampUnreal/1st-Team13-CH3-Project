#include "BUIManager.h"
#include "BGameInstance.h"
#include "BPlayerState.h"
#include "BGameState.h"
#include "BCharacter.h"
#include "BBaseWeapon.h"
#include "BBaseGun.h"
#include "BPistol.h"
#include "NotificationWidget.h"
#include "ItemNotificationWidget.h"
#include "HealthAndLevelWidget.h"
#include "MapWidget.h"
#include "QuickslotWidget.h"
#include "MissionWidget.h"
#include "WeaponAmmoWidget.h"
#include "CrosshairWidget.h"
#include "KillLogWidget.h"
#include "KillCountWidget.h"
#include "BUIWeaponWheel.h"
#include "BInventoryWidget.h"

UBUIManager::UBUIManager()
{
	GameInstance = nullptr;

	/**** Assign WBPs to TSubclassOf variables using FClassFinder ****/
	// Title Screen
	ConstructorHelpers::FClassFinder<UUserWidget> TitleWidgetClassFinder(TEXT("/Game/UI/WBP/WBP_TitleScreen"));
	if (TitleWidgetClassFinder.Succeeded())
	{
		TitleWidgetClass = TitleWidgetClassFinder.Class;
	}

	// Level Transition
	ConstructorHelpers::FClassFinder<UUserWidget> LevelTransitionWidgetClassFinder(TEXT("/Game/UI/WBP/WBP_LevelTransition"));
	if (LevelTransitionWidgetClassFinder.Succeeded())
	{
		LevelTransitionWidgetClass = LevelTransitionWidgetClassFinder.Class;
	}

	// In-Game Menu
	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuWidgetClassFinder(TEXT("/Game/UI/WBP/WBP_InGameMenu"));
	if (InGameMenuWidgetClassFinder.Succeeded())
	{
		InGameMenuWidgetClass = InGameMenuWidgetClassFinder.Class;
	}

	// Game Over
	ConstructorHelpers::FClassFinder<UUserWidget> GameOverWidgetClassFinder(TEXT("/Game/UI/WBP/WBP_GameOverMenu"));
	if (GameOverWidgetClassFinder.Succeeded())
	{
		GameOverWidgetClass = GameOverWidgetClassFinder.Class;
	}

	// HUD
	ConstructorHelpers::FClassFinder<UUserWidget> HUDWidgetClassFinder(TEXT("/Game/UI/WBP/WBP_HUD"));
	if (HUDWidgetClassFinder.Succeeded())
	{
		HUDWidgetClass = HUDWidgetClassFinder.Class;
	}

	// WeaponWheel
	ConstructorHelpers::FClassFinder<UUserWidget> WeaponWheelClassFinder(TEXT("/Game/UI/WBP/WBP_BUIWeaponWheel"));
	if (WeaponWheelClassFinder.Succeeded())
	{
		WeaponWheelClass = WeaponWheelClassFinder.Class;
	}
	ConstructorHelpers::FClassFinder<UUserWidget> Inventory(TEXT("/Game/UI/WBP/WBP_InventoryWidget"));
	if (Inventory.Succeeded())
	{
		InventoryWidget = Inventory.Class;
	}
	TitleWidgetInstance = nullptr;
	LevelTransitionWidgetInstance = nullptr;
	InGameMenuWidgetInstance = nullptr;
	GameOverWidgetInstance = nullptr;
	WeaponWheelInstance = nullptr;
	InventoryWidgetInstance = nullptr;
	HUDWidgetInstance = nullptr;
	NotificationWidget = nullptr;
	ItemNotificationWidget = nullptr;
	HealthAndLevelWidget = nullptr;
	MapWidget = nullptr;
	MissionWidget = nullptr;
	QuickslotWidget = nullptr;
	WeaponAmmoWidget = nullptr;
	CrosshairWidget = nullptr;
	KillLogWidget = nullptr;
	KillCountWidget = nullptr;

	bIsWeaponWheelOpen = false;

	GameInstance = Cast<UBGameInstance>(GetOuter());
}

/**** Widget Instance Getter Methods ****/
UUserWidget* UBUIManager::GetTitleWidgetInstance() { return TitleWidgetInstance; }
UUserWidget* UBUIManager::GetLevelTransitionWidgetInstance() { return LevelTransitionWidgetInstance; }
UUserWidget* UBUIManager::GetInGameMenuWidgetInstance() { return InGameMenuWidgetInstance; }
UUserWidget* UBUIManager::GetGameOverWidgetInstance() { return GameOverWidgetInstance; }
UUserWidget* UBUIManager::GetHUDWidgetInstance() { return HUDWidgetInstance; }
UUserWidget* UBUIManager::GetWeaponWheelInstance() { return WeaponWheelInstance; }

UUserWidget* UBUIManager::GetInventoryInstance()
{
	return InventoryWidgetInstance;
}

/****************** TITLE SCREEN ******************/
// Create Title widget instance
// --> Plays a fade-in animation and activates buttons (WBP Graph OnInitialized)
void UBUIManager::EnterTitleScreen()
{
	if (TitleWidgetClass && TitleWidgetInstance == nullptr)
	{
		if (GameInstance)
		{
			TitleWidgetInstance = CreateWidget<UUserWidget>(GameInstance, TitleWidgetClass);
			if (TitleWidgetInstance)
			{
				TitleWidgetInstance->AddToViewport();
				SetInputUIOnly();
			}
		}
	}
}

// Remove widget instance and change input mode (c.f. Called from WBP on a button click)
void UBUIManager::ExitTitleScreen()
{
	if (TitleWidgetInstance)
	{
		TitleWidgetInstance->RemoveFromParent();
		TitleWidgetInstance = nullptr;
	}
	SetInputGameOnly();
}

/****************** LEVEL TRANSITION ******************/
// Plays a fade-out animation and is removed from parent (WBP Graph Function)
void UBUIManager::LevelStartTransition()
{
	if (LevelTransitionWidgetClass && LevelTransitionWidgetInstance == nullptr)
	{
		if (GameInstance)
		{
			LevelTransitionWidgetInstance = CreateWidget<UUserWidget>(GameInstance, LevelTransitionWidgetClass);
			if (LevelTransitionWidgetInstance)
			{
				LevelTransitionWidgetInstance->AddToViewport();
			}
		}
	}

	// Call WBP Event that plays widget animation and call remove function upon animation finished event
	if (LevelTransitionWidgetInstance)
	{
		if (UFunction* OnLevelStart = LevelTransitionWidgetInstance->FindFunction("OnLevelStart"))
		{
			LevelTransitionWidgetInstance->ProcessEvent(OnLevelStart, nullptr);
		}
	}

	//SetInputUIOnly();
}

// Plays a fade-in animation and is removed from parent (WBP Graph Function)
void UBUIManager::LevelEndTransition()
{
	if (LevelTransitionWidgetClass && LevelTransitionWidgetInstance == nullptr)
	{
		if (GameInstance)
		{
			LevelTransitionWidgetInstance = CreateWidget<UUserWidget>(GameInstance, LevelTransitionWidgetClass);
			if (LevelTransitionWidgetInstance)
			{
				LevelTransitionWidgetInstance->AddToViewport();
			}
		}
	}

	// Call WBP Event that plays widget animation and calls NextLevel upon finished event
	if (LevelTransitionWidgetInstance)
	{
		if (UFunction* OnLevelEnd = LevelTransitionWidgetInstance->FindFunction("OnLevelEnd"))
		{
			LevelTransitionWidgetInstance->ProcessEvent(OnLevelEnd, nullptr);
		}
	}
	SetInputUIOnly();
}

// remove widget instance and set to nullptr
void UBUIManager::RemoveLevelTransition()
{
	if (LevelTransitionWidgetInstance)
	{
		LevelTransitionWidgetInstance->RemoveFromParent();
		LevelTransitionWidgetInstance = nullptr;
	}
	SetInputGameOnly();
}

/****************** IN-GAME MENU ******************/
// Create in-game menu widget instance
// Plays a fade-in animation and activates buttons (WBP Graph OnInitialized)
void UBUIManager::EnterInGameMenu()
{
	if (InGameMenuWidgetClass && InGameMenuWidgetInstance == nullptr)
	{
		if (GameInstance)
		{
			InGameMenuWidgetInstance = CreateWidget<UUserWidget>(GameInstance, InGameMenuWidgetClass);
			if (InGameMenuWidgetInstance)
			{
				InGameMenuWidgetInstance->AddToViewport();
				SetInputUIOnly();
			}
		}
	}
}

// Plays a fade-out animation and is removed from parent (WBP Graph function)
void UBUIManager::ExitInGameMenu()
{
	if (InGameMenuWidgetInstance)
	{
		InGameMenuWidgetInstance->RemoveFromParent();
		InGameMenuWidgetInstance = nullptr;
		SetInputGameOnly();
	}
}

/****************** GAME OVER SCREEN ******************/
// Create game over widget instance
// --> Plays a fade-in animation and activates buttons (WBP Graph OnInitialized)
void UBUIManager::EnterGameOverScreen()
{
	if (GameOverWidgetClass && GameOverWidgetInstance == nullptr)
	{
		if (GameInstance)
		{
			GameOverWidgetInstance = CreateWidget<UUserWidget>(GameInstance, GameOverWidgetClass);
			if (GameOverWidgetInstance)
			{
				GameOverWidgetInstance->AddToViewport();
				SetInputUIOnly();
			}
		}
	}
}

// Remove Instance (WBP Graph Function)
void UBUIManager::ExitGameOverScreen()
{
	if (GameOverWidgetInstance)
	{
		GameOverWidgetInstance->RemoveFromParent();
		GameOverWidgetInstance = nullptr;
		SetInputGameOnly();
	}
}

/****************** WEAPON WHEEL ******************/


// Create weapon wheel widget instance --> animation in WBP
void UBUIManager::EnterWeaponWheel()
{
	if (bIsWeaponWheelOpen) return;

	if (WeaponWheelClass && WeaponWheelInstance == nullptr)
	{
		if (GameInstance)
		{
			WeaponWheelInstance = CreateWidget<UUserWidget>(GameInstance, WeaponWheelClass);
			if (WeaponWheelInstance)
			{
				WeaponWheelInstance->AddToViewport();

				if (UWorld* World = GameInstance->GetWorld())
				{
					if (APlayerController* PlayerController = World->GetFirstPlayerController())
					{
						PlayerController->bShowMouseCursor = true;
						PlayerController->SetInputMode(FInputModeGameAndUI());
					}
				}

				bIsWeaponWheelOpen = true;
			}
		}
	}
}

// Remove instance (To be called when fade out animation is finished)
void UBUIManager::ExitWeaponWheel()
{
	if (!bIsWeaponWheelOpen) return;

	if (WeaponWheelInstance)
	{
		if (UBUIWeaponWheel* WeaponWheelWidget = Cast<UBUIWeaponWheel>(WeaponWheelInstance))
		{
			WeaponWheelWidget->ExitWeaponWheel();
		}
	}
}

void UBUIManager::RemoveWeaponWheel()
{
	if (WeaponWheelInstance)
	{
		WeaponWheelInstance->RemoveFromParent();
		WeaponWheelInstance = nullptr;
		SetInputGameOnly();
		bIsWeaponWheelOpen = false;
	}
}

/****************** Switch Input Mode & Mouse Cursor ******************/
void UBUIManager::SetInputUIOnly()
{
	if (GameInstance)
	{
		if (UWorld* World = GameInstance->GetWorld())
		{
			if (APlayerController* PlayerController = World->GetFirstPlayerController())
			{
				PlayerController->bShowMouseCursor = true;
				PlayerController->SetInputMode(FInputModeUIOnly());
			}
		}
	}
}

void UBUIManager::SetInputGameOnly()
{
	if (GameInstance)
	{
		if (UWorld* World = GameInstance->GetWorld())
		{
			if (APlayerController* PlayerController = World->GetFirstPlayerController())
			{
				PlayerController->bShowMouseCursor = false;
				PlayerController->SetInputMode(FInputModeGameOnly());
			}
		}
	}
}

void UBUIManager::SetInputUIAndGame()
{
	if (GameInstance)
	{
		if (UWorld* World = GameInstance->GetWorld())
		{
			if (APlayerController* PlayerController = World->GetFirstPlayerController())
			{
				PlayerController->bShowMouseCursor = true;
				PlayerController->SetInputMode(FInputModeGameAndUI());
			}
		}
	}
}

/****************** HUD ******************/
void UBUIManager::DisplayHUD()
{
	/**** Create and display HUD widget instance ****/
	if (HUDWidgetInstance == nullptr)
	{
		if (HUDWidgetClass && GameInstance)
		{
			HUDWidgetInstance = CreateWidget<UUserWidget>(GameInstance, HUDWidgetClass);
			if (HUDWidgetInstance)
			{
				HUDWidgetInstance->AddToViewport();
			}
		}
	}

	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->SetVisibility(ESlateVisibility::HitTestInvisible);
		// TODO: 페이드 인

		// GetWidgetFromName
		CrosshairWidget = Cast<UCrosshairWidget>(HUDWidgetInstance->GetWidgetFromName(TEXT("CrosshairWidget")));
		HealthAndLevelWidget = Cast<UHealthAndLevelWidget>(HUDWidgetInstance->GetWidgetFromName(TEXT("HealthAndLevelWidget")));
		MapWidget = Cast<UMapWidget>(HUDWidgetInstance->GetWidgetFromName(TEXT("MapWidget")));
		QuickslotWidget = Cast<UQuickslotWidget>(HUDWidgetInstance->GetWidgetFromName(TEXT("QuickslotWidget")));
		WeaponAmmoWidget = Cast<UWeaponAmmoWidget>(HUDWidgetInstance->GetWidgetFromName(TEXT("WeaponAmmoWidget")));
		NotificationWidget = Cast<UNotificationWidget>(HUDWidgetInstance->GetWidgetFromName(TEXT("NotificationWidget")));
		ItemNotificationWidget = Cast<UItemNotificationWidget>(HUDWidgetInstance->GetWidgetFromName(TEXT("ItemNotificationWidget")));
		MissionWidget = Cast<UMissionWidget>(HUDWidgetInstance->GetWidgetFromName(TEXT("MissionWidget")));
		KillLogWidget = Cast<UKillLogWidget>(HUDWidgetInstance->GetWidgetFromName(TEXT("KillLogWidget")));
		KillCountWidget = Cast<UKillCountWidget>(HUDWidgetInstance->GetWidgetFromName(TEXT("KillCountWidget")));

		UpdateHUD();
	}
}

void UBUIManager::UpdateHUD()
{
	if (GetWorld())
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (UInputComponent* PlayerInputComponent = PlayerController->InputComponent)
			{
				PlayerInputComponent->BindAction("WeaponWheel", IE_Pressed, this, &UBUIManager::EnterWeaponWheel);
				PlayerInputComponent->BindAction("WeaponWheel", IE_Released, this, &UBUIManager::ExitWeaponWheel);
			}

			if (ABPlayerState* PlayerState = PlayerController->GetPlayerState<ABPlayerState>())
			{
				UpdateHUDHealth(PlayerState->GetCurrentHealth(), PlayerState->GetMaxHealth());
				UpdateHUDLevelAndExp(PlayerState->GetPlayerLevel(), PlayerState->GetCurrentExp(), PlayerState->GetMaxExp());

				if (ABCharacter* Character = Cast<ABCharacter>(PlayerController->GetCharacter()))
				{
					if (ABBaseWeapon* EquippedWeapon = Character->GetCurrentWeapon())
					{
						UpdateHUDEquippedWeapon(EquippedWeapon->WeaponType);
						if (EquippedWeapon->IsA<ABBaseGun>())
						{
							ABBaseGun* EquippedGun = Cast<ABBaseGun>(EquippedWeapon);
							UpdateHUDLoadedAmmo(EquippedGun->AmmoCount);

							if (EquippedWeapon->IsA<ABPistol>())
							{
								UpdateHUDInventoryAmmo();
							}
							else
							{
								// TODO: check Ammo ItemName 
								TArray<FItemData> InventoryAmmo = PlayerState->GetInventoryTypeItem(FName(EquippedWeapon->WeaponType + "Ammo"));
								UpdateHUDInventoryAmmo(InventoryAmmo.Num());
							}
						}
						else
						{
							UpdateHUDLoadedAmmo();
							UpdateHUDInventoryAmmo();
						}
					}
					TArray<FItemData> InventoryFirstAidKit = PlayerState->GetInventoryTypeItem(FName("FirstAidKit"));
					UpdateHUDQuickSlot("FirstAidKit", InventoryFirstAidKit.Num());
					TArray<FItemData> InventoryGrenade = PlayerState->GetInventoryTypeItem(FName("Grenade"));
					UpdateHUDQuickSlot("Grenade", InventoryFirstAidKit.Num());
				}
			}
		}

		if (GameInstance)
		{
			UpdateCurrentScore(GameInstance->GetScore());
		}

		if (ABGameState* GameState = GetWorld()->GetGameState<ABGameState>())
		{
			UpdateKillCount(GameState->KilledEnemies);
		}
	}

	UpdateHUDTimed();
}

void UBUIManager::CollapseHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UBUIManager::RemoveHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
}

void UBUIManager::ShowInventory()
{
	if (InventoryWidgetInstance == nullptr)
	{
		if (InventoryWidget && GameInstance)
		{
			UUserWidget* UserWidget = CreateWidget<UUserWidget>(GameInstance, InventoryWidget);
			InventoryWidgetInstance = Cast<UBInventoryWidget>(UserWidget);
			if (InventoryWidgetInstance)
			{
				InventoryWidgetInstance->AddToViewport();
			}
		}
	}
	if (GameInstance)
	{
		if (UWorld* World = GameInstance->GetWorld())
		{
			if (APlayerController* PlayerController = World->GetFirstPlayerController())
			{
				PlayerController->SetPause(false);
				PlayerController->bShowMouseCursor = true;
				PlayerController->SetInputMode(FInputModeGameAndUI());
			}
		}
	}
}

void UBUIManager::CloseInventory()
{
	if (InventoryWidgetInstance)
	{
		InventoryWidgetInstance->RemoveFromParent();
		InventoryWidgetInstance = nullptr;
	}
	if (GameInstance)
	{
		if (UWorld* World = GameInstance->GetWorld())
		{
			if (APlayerController* PlayerController = World->GetFirstPlayerController())
			{
				PlayerController->SetPause(false);
				PlayerController->bShowMouseCursor = false;
				PlayerController->SetInputMode(FInputModeGameOnly());
			}
		}
	}
}

// Update timed elements in HUD (repeated by UpdateHUDTimerHandle)
void UBUIManager::UpdateHUDTimed()
{
	if (GetWorld() && !GetWorld()->bIsTearingDown)
	{
		GetWorld()->GetTimerManager().SetTimer(
			UpdateHUDTimerHandle,
			this,
			&UBUIManager::UpdateHUDMap,
			0.1f,
			true
		);
	}
}

void UBUIManager::UpdateHUDMap()
{
	if (GetWorld() == nullptr || GetWorld()->bIsTearingDown) return;

	if (HUDWidgetInstance && MapWidget)
	{
		MapWidget->UpdateMap();
	}
}

void UBUIManager::UpdateHUDItemMission(const FName& ItemName, const int32& CurrentCount, const int32& TargetCount)
{
	if (HUDWidgetInstance && MissionWidget)
	{
		MissionWidget->UpdateItemMission(ItemName, CurrentCount, TargetCount);
	}
}

void UBUIManager::UpdateHUDBonusMission(const int32& CurrentCount, const int32& TargetCount)
{
	if (HUDWidgetInstance && MissionWidget)
	{
		MissionWidget->UpdateBonusMission(CurrentCount, TargetCount);
	}
}

void UBUIManager::RemoveAllMissions()
{
	if (HUDWidgetInstance && MissionWidget)
	{
		MissionWidget->RemoveAllMissions();
	}
}

void UBUIManager::UpdateKillLog(const FName& KilledName)
{
	if (HUDWidgetInstance && KillLogWidget)
	{
		//if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		//{
		//	if (ABCharacter* Character = Cast<ABCharacter>(PlayerController->GetCharacter()))
		//	{
		//		if (ABBaseWeapon* EquippedWeapon = Character->GetCurrentWeapon())
		//		{
		KillLogWidget->UpdateKillLog(KilledName);
		//}
	//}
//}
	}
}

void UBUIManager::UpdateKillCount(const int32& KillCount)
{
	if (HUDWidgetInstance && KillCountWidget)
	{
		KillCountWidget->UpdateKillCount(KillCount);
	}
}

void UBUIManager::UpdateCurrentScore(const int32& CurrentScore)
{
	if (HUDWidgetInstance && KillCountWidget)
	{
		KillCountWidget->UpdateCurrentScore(CurrentScore);
	}
}

void UBUIManager::UpdateHUDHealth(const float& CurrentHP, const float& MaxHP)
{
	if (HUDWidgetInstance && HealthAndLevelWidget)
	{
		HealthAndLevelWidget->UpdateHealth(CurrentHP, MaxHP);
	}
}

void UBUIManager::UpdateHUDLevelAndExp(const int32& PlayerLevel, const float& CurrentExp, const float& MaxExp)
{
	if (HUDWidgetInstance && HealthAndLevelWidget)
	{
		HealthAndLevelWidget->UpdateLevelAndExp(PlayerLevel, CurrentExp, MaxExp);
	}
}

void UBUIManager::UpdateHUDQuickSlot(const FName& ItemName, const int32& Count)
{
	if (HUDWidgetInstance && QuickslotWidget)
	{
		QuickslotWidget->UpdateQuickslot(ItemName, Count);
	}
}

void UBUIManager::UpdateHUDLoadedAmmo(const int32& LoadedAmmo)
{
	if (HUDWidgetInstance && WeaponAmmoWidget)
	{
		WeaponAmmoWidget->UpdateLoadedAmmo(LoadedAmmo);
	}
}

void UBUIManager::UpdateHUDInventoryAmmo(const int32& InventoryAmmo)
{
	if (HUDWidgetInstance && WeaponAmmoWidget)
	{
		WeaponAmmoWidget->UpdateInventoryAmmo(InventoryAmmo);
	}
}


void UBUIManager::UpdateHUDEquippedWeapon(const FString& WeaponType)
{
	if (HUDWidgetInstance && WeaponAmmoWidget)
	{
		WeaponAmmoWidget->UpdateWeapon(WeaponType);
	}
}

void UBUIManager::DisplayNotification(const FString& Title, const FString& Message)
{
	if (HUDWidgetInstance && NotificationWidget)
	{
		NotificationWidget->DisplayNotification(Title, Message);
	}
}

void UBUIManager::DisplayItemNotification(const FName& ItemName)
{
	if (HUDWidgetInstance && ItemNotificationWidget)
	{
		ItemNotificationWidget->DisplayNotification(ItemName);
	}
}

void UBUIManager::OnFire()
{
	if (HUDWidgetInstance && CrosshairWidget)
	{
		CrosshairWidget->OnFire();
	}
}

void UBUIManager::OnNormalHit()
{
	if (HUDWidgetInstance && CrosshairWidget)
	{
		CrosshairWidget->OnNormalHit();
	}
}

TTuple<FVector, FVector> UBUIManager::GetCrosshairLocationAndDirection()
{
	FVector CrosshairLocation = FVector::ZeroVector;
	FVector CrosshairDirection = FVector::ZeroVector;
	if (APlayerController* PlayerController = GameInstance->GetFirstLocalPlayerController())
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		PlayerController->DeprojectScreenPositionToWorld(
			0.5f * ViewportSize.X, 0.5f * ViewportSize.Y, CrosshairLocation, CrosshairDirection);
	}
	return TTuple<FVector, FVector>(CrosshairLocation, CrosshairDirection);
}