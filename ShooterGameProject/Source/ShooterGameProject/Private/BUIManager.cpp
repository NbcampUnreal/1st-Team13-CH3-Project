#include "BUIManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "BGameInstance.h"

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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Title ClassFinder Failed"))
	}

	// Level Transition
	ConstructorHelpers::FClassFinder<UUserWidget> LevelTransitionWidgetClassFinder(TEXT("/Game/UI/WBP_LevelTransition"));
	if (LevelTransitionWidgetClassFinder.Succeeded())
	{
		LevelTransitionWidgetClass = LevelTransitionWidgetClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelTransition ClassFinder Failed"))
	}

	// In-Game Menu
	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuWidgetClassFinder(TEXT("/Game/UI/WBP_InGameMenu"));
	if (InGameMenuWidgetClassFinder.Succeeded())
	{
		InGameMenuWidgetClass = InGameMenuWidgetClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InGameMenu ClassFinder Failed"))
	}

	// Game Over
	ConstructorHelpers::FClassFinder<UUserWidget> GameOverWidgetClassFinder(TEXT("/Game/UI/WBP_GameOverMenu"));
	if (GameOverWidgetClassFinder.Succeeded())
	{
		GameOverWidgetClass = GameOverWidgetClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameOver ClassFinder Failed"))
	}

	// HUD
	ConstructorHelpers::FClassFinder<UUserWidget> HUDWidgetClassFinder(TEXT("/Game/UI/WBP_HUD"));
	if (HUDWidgetClassFinder.Succeeded())
	{
		HUDWidgetClass = HUDWidgetClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD ClassFinder Failed"))
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
				UE_LOG(LogTemp, Warning, TEXT("LevelTransitionInstanced"))
			}
		}
	}

	// Call WBP Event that plays widget animation and call remove function upon animation finished event
	if (LevelTransitionWidgetInstance)
	{
		if (UFunction* OnLevelStart = LevelTransitionWidgetInstance->FindFunction("OnLevelStart"))
		{
			UE_LOG(LogTemp, Warning, TEXT("ProcessEvent"))
			LevelTransitionWidgetInstance->ProcessEvent(OnLevelStart, nullptr);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("No Function"))
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
			UE_LOG(LogTemp, Warning, TEXT("Processed Function"))
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
		UE_LOG(LogTemp, Warning, TEXT("InstanceRemoved"))
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

/****************** HUD ******************/
void UBUIManager::UpdateHUD()
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
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("HUD CreateWidget Failed"))
			}
		}
	}

	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->SetVisibility(ESlateVisibility::HitTestInvisible);

		/**** Update HUD elements ****/
		// HP
		if (UTextBlock* CurrentHPText = Cast<UTextBlock>(HUDWidgetInstance->GetWidgetFromName(TEXT("CurrentHPText"))))
		{

		}
	}
}

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

