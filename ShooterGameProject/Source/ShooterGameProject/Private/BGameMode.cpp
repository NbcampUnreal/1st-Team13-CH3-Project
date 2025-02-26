#include "BGameMode.h"
#include "BGameState.h"
#include "BPlayerController.h"
#include "BCharacter.h"
#include "BGameInstance.h"
#include "BSpawnVolume.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


ABGameMode::ABGameMode()
{
	DefaultPawnClass = ABCharacter::StaticClass();
	PlayerControllerClass = ABPlayerController::StaticClass();
	GameStateClass = ABGameState::StaticClass();
	PlayerStateClass = ABPlayerState::StaticClass();
}

void ABGameMode::BeginPlay()
{
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
		StartLevel();
	}	
	UE_LOG(LogTemp, Warning, TEXT("GameMode BeginPlay"));
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
	StartGame();
}
void ABGameMode::SpawnLevelKeyBox()
{
	if (!KeyBoxClass)
	{
		UE_LOG(LogTemp, Error, TEXT("KeyBoxClass is NULL! Check if it is set in BP_BGameMode."));
		return;
	}

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

	ABSpawnVolume* SpawnVolume = Cast<ABSpawnVolume>(SpawnVolumes[0]);
	if (!SpawnVolume) return;

	for (int32 i = 0; i < RequiredKeyCount; i++)
	{
		AActor* SpawnedKeyBox = SpawnVolume->SpawnKeyBox(KeyBoxClass);
		if (!SpawnedKeyBox)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn KeyBox!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("KeyBox spawned at %s"), *SpawnedKeyBox->GetActorLocation().ToString());
		}
	}
}

void ABGameMode::StartGame()
{
	UE_LOG(LogTemp, Log, TEXT("Start! Eliminate all the enemies!"));
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
			UIManager->DisplayHUD();
		}
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
	if (UBGameInstance* BGameInstance = GetGameInstance<UBGameInstance>())
	{
		int32 CurrentStage = BGameInstance->GetCurrentStage() + 1;
		BGameInstance->SetCurrentStage(CurrentStage);
		UE_LOG(LogTemp, Log, TEXT("Loading next level: %d"), CurrentStage);

		FName NextLevelName = "MainLevel";
		if (CurrentStage == 3)
		{
			if (FMath::RandRange(0, 100) < 90) // 3번째 스테이지는 90% 확률로 보너스 레벨 맵
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
		GameInstance->HasTitleScreenBeenShown = false;
		if (UBUIManager* UIManager = GameInstance->GetUIManagerInstance())
		{
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