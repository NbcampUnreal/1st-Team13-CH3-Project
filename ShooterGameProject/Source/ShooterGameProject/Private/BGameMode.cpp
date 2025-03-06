#include "BGameMode.h"
#include "BGameState.h"
#include "BPlayerController.h"
#include "BCharacter.h"
#include "BGameInstance.h"
#include "BSpawnVolume.h"
#include "Kismet/GameplayStatics.h"
#include "BPlayerState.h"
#include "SkyManager.h"
#include "TimerManager.h"
#include "BEnemySpawnVolume.h"

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
}

void ABGameMode::StartLevel()
{
	ABGameState* BGameState = GetGameState<ABGameState>();
	if (BGameState)
	{
		BGameState->InitializeGameState();
	}
	SpawnLevelKeyBox();

	UBGameInstance* GameInstance = Cast<UBGameInstance>(GetGameInstance()); //resetHUD
	if (GameInstance)
	{
		GameInstance->GetUIManagerInstance()->RemoveHUD();
	}

	StartGame();
}
void ABGameMode::SpawnLevelKeyBox()
{
	ABGameState* BGameState = GetGameState<ABGameState>();
	if (!BGameState) return;

	//int32 RequiredKeyCount = BGameState->RequiredKeyCount;

	TArray<AActor*> SpawnVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABSpawnVolume::StaticClass(), SpawnVolumes);

	if (SpawnVolumes.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No ABSpawnVolume found in the level!"));
		return;
	}


	for (int32 i = 0; i < 3; i++)
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
	StartEnemySpawning();
	ABGameState* BGameState = GetGameState<ABGameState>();
	/*
	GetWorldTimerManager().SetTimer
	(
		LevelTimerHandle,
		this,
		&ABGameMode::EndGame,
		BGameState->TimeLimit,
		false
	);*/

	if (UBGameInstance* GameInstance = GetGameInstance<UBGameInstance>())
	{
		ASkyManager* SkyManager = Cast<ASkyManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ASkyManager::StaticClass()));
		if (SkyManager)
		{
			SkyManager->UpdateSkyColor(GameInstance->GetCurrentStage());
		}
		if (UBUIManager* UIManager = GameInstance->GetUIManagerInstance())
		{
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
	}
}

void ABGameMode::NextLevel() //다음 레벨로 이동
{
	if (UBGameInstance* BGameInstance = GetGameInstance<UBGameInstance>())
	{
		int32 CurrentStage = BGameInstance->GetCurrentStage() + 1;
		BGameInstance->SetCurrentStage(CurrentStage);
		UE_LOG(LogTemp, Log, TEXT("Loading next level: %d"), CurrentStage);

		FName NextLevelName = "1stLevel";
		if (CurrentStage == 2)
		{
				NextLevelName = "BonusLevel";
		}
		if (CurrentStage == 3)
		{
			NextLevelName = "2ndLevel";
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

void ABGameMode::StartEnemySpawning()
{
	UBGameInstance* GameInstance = Cast<UBGameInstance>(GetGameInstance());
	if (!GameInstance) return;

	int32 CurrentStage = GameInstance->GetCurrentStage();

	int32 TotalBasicCount = (CurrentStage == 3) ? 30 : 10;
	int32 TotalSkillCount = (CurrentStage == 3) ? 0 : 4;

	TArray<AActor*> EnemySpawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABEnemySpawnVolume::StaticClass(), EnemySpawners);

	if (EnemySpawners.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("EnemySpawnVolume 없음!"));
		return;
	}

	int32 NumSpawners = EnemySpawners.Num();
	int32 BasicPerSpawner = TotalBasicCount / NumSpawners;
	int32 SkillPerSpawner = TotalSkillCount / NumSpawners;

	int32 BasicRemainder = TotalBasicCount % NumSpawners;
	int32 SkillRemainder = TotalSkillCount % NumSpawners;

	for (AActor* SpawnerActor : EnemySpawners)
	{
		if (ABEnemySpawnVolume* Spawner = Cast<ABEnemySpawnVolume>(SpawnerActor))
		{
			int32 AssignedBasic = BasicPerSpawner + (BasicRemainder > 0 ? 1 : 0);
			int32 AssignedSkill = SkillPerSpawner + (SkillRemainder > 0 ? 1 : 0);

			if (BasicRemainder > 0) BasicRemainder--;
			if (SkillRemainder > 0) SkillRemainder--;

			UE_LOG(LogTemp, Warning, TEXT("스폰 볼륨 %s에서 Basic %d마리, Skill %d마리 스폰"),
				*Spawner->GetName(), AssignedBasic, AssignedSkill);

			Spawner->SpawnEnemies(AssignedBasic, AssignedSkill);
		}
	}
}