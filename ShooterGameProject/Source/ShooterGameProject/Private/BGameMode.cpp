#include "BGameMode.h"
#include "BGameState.h"
#include "BPlayerController.h"
#include "BCharacter.h"
#include "BGameInstance.h"
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
	if (!GameInstance->HasTitleScreenBeenShown)
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

	StartGame();
}

void ABGameMode::StartGame()
{
	UE_LOG(LogTemp, Log, TEXT("Start! Eliminate all the enemies!"));
	ABGameState* BGameState = GetGameState<ABGameState>();

	GetWorldTimerManager().SetTimer
	(
		LevelTimerHandle,
		this,
		//&ABGameMode::EndGame,
		&ABGameMode::NextLevel,
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

		FName NextLevelName = "StartLevel";
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
	UGameplayStatics::OpenLevel(this, "StartLevel"); // 메인레벨로 가서 메뉴띄우기
}

void ABGameMode::QuitGame()
{
	UWorld* World = GetWorld();
	UKismetSystemLibrary::QuitGame(World, nullptr, EQuitPreference::Quit, false);
}