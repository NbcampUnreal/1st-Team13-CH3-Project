#include "BGameMode.h"
#include "BGameState.h"
#include "BPlayerController.h"
#include "BCharacter.h"
#include "BGameInstance.h"
#include "BSpawnVolume.h"
#include "Kismet/GameplayStatics.h"
#include "BPlayerState.h"
#include "TimerManager.h"


ABGameMode::ABGameMode()
{
	UE_LOG(LogTemp, Warning, TEXT("ABGameMode::ABGameMode()"));
	DefaultPawnClass = ABCharacter::StaticClass();
	PlayerControllerClass = ABPlayerController::StaticClass();
	GameStateClass = ABGameState::StaticClass();
	PlayerStateClass = ABPlayerState::StaticClass();
}

void ABGameMode::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("GameMode BeginPlay start"));
	Super::BeginPlay();

	UBGameInstance* GameInstance = Cast<UBGameInstance>(GetGameInstance());
	if (GameInstance && !GameInstance->HasTitleScreenBeenShown)
	{
		GameInstance->HasTitleScreenBeenShown = true;
		GameInstance->GetUIManagerInstance()->EnterTitleScreen();
	}
	else
	{
		GameInstance->GetUIManagerInstance()->LevelStartTransition();
		UE_LOG(LogTemp, Warning, TEXT("ABGameMode::BeginPlay().StartLevel()"));
		StartLevel();
	}	
	UE_LOG(LogTemp, Warning, TEXT("GameMode BeginPlay end"));
}

void ABGameMode::StartLevel()
{
	ABGameState* BGameState = GetGameState<ABGameState>();
	if (BGameState)
	{
		BGameState->InitializeGameState();
	}
	UE_LOG(LogTemp, Warning, TEXT("StartLevel: Spawning KeyBoxes!"));
	SpawnLevelKeyBox();

	UBGameInstance* GameInstance = Cast<UBGameInstance>(GetGameInstance()); //resetHUD
	if (GameInstance)
	{
		UE_LOG(LogTemp, Log, TEXT("StartGame().RemoveHUD"));
		GameInstance->GetUIManagerInstance()->RemoveHUD();
	}

	StartGame();
}
void ABGameMode::SpawnLevelKeyBox()
{
	ABGameState* BGameState = GetGameState<ABGameState>();
	if (!BGameState) return;

	int32 RequiredKeyCount = BGameState->RequiredKeyCount;

	TArray<AActor*> SpawnVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABSpawnVolume::StaticClass(), SpawnVolumes);

	if (SpawnVolumes.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No ABSpawnVolume found in the level!"));
		return;
	}


	for (int32 i = 0; i < RequiredKeyCount; i++)
	{
        int32 RandomIndex = FMath::RandRange(0, SpawnVolumes.Num() - 1);
        ABSpawnVolume* SpawnVolume = Cast<ABSpawnVolume>(SpawnVolumes[RandomIndex]);

        if (SpawnVolume)
        {
            SpawnVolume->SpawnKeyBox(KeyBoxClass);
        }
	}
}

void ABGameMode::StartGame()
{
	UE_LOG(LogTemp, Log, TEXT("StartGame! Eliminate all the enemies!"));
	ABGameState* BGameState = GetGameState<ABGameState>();

	GetWorldTimerManager().SetTimer
	(
		LevelTimerHandle,
		this,
		&ABGameMode::EndGame,
		BGameState->TimeLimit,
		false
	);

	if (UBGameInstance* GameInstance = GetGameInstance<UBGameInstance>())
	{
		if (UBUIManager* UIManager = GameInstance->GetUIManagerInstance())
		{
			UE_LOG(LogTemp, Log, TEXT("StartGame().DisplayHUD()"));
						UIManager->DisplayHUD();
			}
	}
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}

}

void ABGameMode::onDoorReached() //문에 플레이어 도달 시 호출
{
	ABGameState* BGameState = GetGameState<ABGameState>();
	if (BGameState->bIsDoorOpen)
	{
		UBGameInstance* GameInstance = Cast<UBGameInstance>(GetGameInstance());
		if (GameInstance)
		{
			GameInstance->GetUIManagerInstance()->LevelEndTransition();
		}
		NextLevel();
	}
}

void ABGameMode::NextLevel() //다음 레벨로 이동
{
	UE_LOG(LogTemp, Log, TEXT("StartNextLevel()"));

	if (UBGameInstance* BGameInstance = GetGameInstance<UBGameInstance>())
	{
		int32 CurrentStage = BGameInstance->GetCurrentStage() + 1;
		BGameInstance->SetCurrentStage(CurrentStage);
		UE_LOG(LogTemp, Log, TEXT("Loading next level: %d"), CurrentStage);

		FName NextLevelName = "MainLevel";
		if (CurrentStage % 3 == 0)
		{
			if (FMath::RandRange(0, 100) < 90)
			{
				NextLevelName = "BonusLevel";
			}
		}

		UE_LOG(LogTemp, Log, TEXT("Loading next level: %s"), *NextLevelName.ToString());
		UGameplayStatics::OpenLevel(this, NextLevelName);
	}
}

void ABGameMode::EndGame()
{
	if (ABGameState* BGameState = GetGameState<ABGameState>())
	{
		BGameState->TriggerGameOver();
	}
}

void ABGameMode::RestartGame()
{
	UBGameInstance* GameInstance = Cast<UBGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->SetCurrentStage(0);
		GameInstance->HasTitleScreenBeenShown = false;
		if (UBUIManager* UIManager = GameInstance->GetUIManagerInstance())
		{
			UE_LOG(LogTemp, Log, TEXT("RestartGame().RemoveHUD"));
			UIManager->RemoveHUD();
		}
	}
	UGameplayStatics::OpenLevel(this, "StartLevel");
}

void ABGameMode::QuitGame()
{
	UWorld* World = GetWorld();
	UKismetSystemLibrary::QuitGame(World, nullptr, EQuitPreference::Quit, false);
}