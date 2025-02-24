#include "BUIManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "BGameInstance.h"
#include "BPlayerController.h"
#include "BCharacter.h"

UBUIManager::UBUIManager()
{
	GameInstance = nullptr;

	/**** Assign WBPs to TSubclassOf variables using FClassFinder ****/
	// Title Screen
	ConstructorHelpers::FClassFinder<UUserWidget> TitleWidgetClassFinder(TEXT("/Game/UI/WBP_TitleScreen"));
	if (TitleWidgetClassFinder.Succeeded())
	{
		TitleWidgetClass = TitleWidgetClassFinder.Class;
	}

	// Level Transition
	ConstructorHelpers::FClassFinder<UUserWidget> LevelTransitionWidgetClassFinder(TEXT("/Game/UI/WBP_LevelTransition"));
	if (LevelTransitionWidgetClassFinder.Succeeded())
	{
		LevelTransitionWidgetClass = LevelTransitionWidgetClassFinder.Class;
	}

	// In-Game Menu
	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuWidgetClassFinder(TEXT("/Game/UI/WBP_InGameMenu"));
	if (InGameMenuWidgetClassFinder.Succeeded())
	{
		InGameMenuWidgetClass = InGameMenuWidgetClassFinder.Class;
	}

	// Game Over
	ConstructorHelpers::FClassFinder<UUserWidget> GameOverWidgetClassFinder(TEXT("/Game/UI/WBP_GameOverMenu"));
	if (GameOverWidgetClassFinder.Succeeded())
	{
		GameOverWidgetClass = GameOverWidgetClassFinder.Class;
	}

	// HUD
	ConstructorHelpers::FClassFinder<UUserWidget> HUDWidgetClassFinder(TEXT("/Game/UI/WBP_HUD"));
	if (HUDWidgetClassFinder.Succeeded())
	{
		HUDWidgetClass = HUDWidgetClassFinder.Class;
	}
	
	/**** Get and assign textures ****/
	ConstructorHelpers::FObjectFinder<UTexture2D> PistolIconFinder(TEXT("/Game/UI/Textures/T_Pistol_Icon.T_Pistol_Icon"));
	if (PistolIconFinder.Succeeded())
	{
		PistolIconTexture = PistolIconFinder.Object;
	}

	ConstructorHelpers::FObjectFinder<UTexture2D> RifleIconFinder(TEXT("/Game/UI/Textures/T_Rifle_Icon.T_Rifle_Icon"));
	if (RifleIconFinder.Succeeded())
	{
		RifleIconTexture  = RifleIconFinder.Object;
	}

	TitleWidgetInstance = nullptr;
	LevelTransitionWidgetInstance = nullptr;
	InGameMenuWidgetInstance = nullptr;
	GameOverWidgetInstance = nullptr;
	HUDWidgetInstance = nullptr;
}

// Create HUD widget when properties are initialized
void UBUIManager::PostInitProperties()
{
	Super::PostInitProperties();

	GameInstance = Cast<UBGameInstance>(GetOuter());
}

/**** Widget Instance Getter Methods ****/
UUserWidget* UBUIManager::GetTitleWidgetInstance() { return TitleWidgetInstance; }
UUserWidget* UBUIManager::GetLevelTransitionWidgetInstance() { return LevelTransitionWidgetInstance; }
UUserWidget* UBUIManager::GetInGameMenuWidgetInstance() { return InGameMenuWidgetInstance; }
UUserWidget* UBUIManager::GetGameOverWidgetInstance() { return GameOverWidgetInstance; }
UUserWidget* UBUIManager::GetHUDWidgetInstance() { return HUDWidgetInstance; }

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

	SetInputUIOnly();
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

	// Call WBP Event that plays widget animation and removes the widget upon animation finished event
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

// Plays a fade-out animation and is removed from parent (WBP Graph Function)
void UBUIManager::ExitGameOverScreen()
{
	if (GameOverWidgetInstance)
	{
		GameOverWidgetInstance->RemoveFromParent();
		GameOverWidgetInstance = nullptr;
		SetInputGameOnly();
	}
}

/****************** Input Mode & Mouse Cursor ******************/
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

/****************** HUD ******************/
// Crosshair animation when fired
void UBUIManager::OnFire()
{
	if (HUDWidgetInstance)
	{
		if (UFunction* OnFireFunc = HUDWidgetInstance->FindFunction(TEXT("OnFire")))
		{
			HUDWidgetInstance->ProcessEvent(OnFireFunc, nullptr);
		}
	}
}

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
	}
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

// Update timed elements in HUD (repeated by UpdateHUDTimerHandle)
void UBUIManager::UpdateHUDTimed()
{
	// UpdateHUDStatus();
	// UpdateHUDLevelTimer();
	// UpdateHUDMap(); 호출??
}

void UBUIManager::UpdateHUDHealth(float CurrentHP, float MaxHP)
{
	if (MaxHP == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("!UpdateHUDHealth received 0 as MaxHP!"));
		return;
	}

	if (HUDWidgetInstance)
	{
		// HP Numbers
		if (UTextBlock* CurrentHPText = Cast<UTextBlock>(HUDWidgetInstance->GetWidgetFromName(TEXT("CurrentHPText"))))
		{
			CurrentHPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CurrentHP)));
		}
		if (UTextBlock* MaxHPText = Cast<UTextBlock>(HUDWidgetInstance->GetWidgetFromName(TEXT("MaxHPText"))))
		{
			MaxHPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), MaxHP)));
		}

		// HP Bar
		if (UProgressBar* HPBar = Cast<UProgressBar>(HUDWidgetInstance->GetWidgetFromName(TEXT("HPBar"))))
		{
			float HPPercent = CurrentHP / MaxHP;
			HPBar->SetPercent(HPPercent);
		}
	}
}

//void UBUIManager::UpdateHUDStatus()
//{
	// TODO: check character status & remaining time
//}

//void UBUIManager::UpdateHUDLevelTimer()
//{
	// TODO: check level timer remaining time
//}

void UBUIManager::UpdateHUDQuickSlot(FName ItemName, int32 Count)
{
	// TODO: Based on ItemType, change the count of that item in quick slot
	FName WidgetName = "";
	if (ItemName == "FirstAidKit")
	{
		WidgetName = "QuickSlotCountText_0";
	}

	if (HUDWidgetInstance)
	{
		if (UTextBlock* QuickSlotText = Cast<UTextBlock>(HUDWidgetInstance->GetWidgetFromName(WidgetName)))
		{
			QuickSlotText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Count)));
		}
	}
}

void UBUIManager::UpdateHUDAmmo(int32 LoadedCount, int32 InventoryCount)
{
	if (HUDWidgetInstance)
	{
		// Count of ammo loaded into current weapon
		if (UTextBlock* LoadedAmmoText = Cast<UTextBlock>(HUDWidgetInstance->GetWidgetFromName(TEXT("LoadedAmmoText"))))
		{
			LoadedAmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d"), LoadedCount)));
		}
		if (UTextBlock* InventoryAmmoText = Cast<UTextBlock>(HUDWidgetInstance->GetWidgetFromName(TEXT("InventoryAmmoText"))))
		{
			InventoryAmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d"), InventoryCount)));
		}
	}
}

void UBUIManager::UpdateHUDEquippedWeapon(FName WeaponType)
{
	// TODO: Based on the equipped weapon, change the icon / thumbnail
	UTexture2D* WeaponTexture = nullptr;
	if (WeaponType == "Pistol" && IsValid(PistolIconTexture))
	{
		WeaponTexture = PistolIconTexture;
	}
	else if (WeaponType == "Rifle" && IsValid(RifleIconTexture))
	{
		WeaponTexture = RifleIconTexture;
	}

	if (HUDWidgetInstance)
	{
		if (UImage* WeaponImage = Cast<UImage>(HUDWidgetInstance->GetWidgetFromName(TEXT("WeaponImage"))))
		{
			if (WeaponTexture)
			{
				WeaponImage->SetBrushFromTexture(WeaponTexture);
			}
		}
	}
}



