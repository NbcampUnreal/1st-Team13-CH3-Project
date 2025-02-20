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
	}
}

void ABGameMode::StartLevel()
{
	ABGameState* BGameState = GetGameState<ABGameState>();
	if (BGameState)
	{
		BGameState->SpawnedEnemies = 10; //스폰할 적 수
		BGameState->KilledEnemies = 0;
		BGameState->CollectedKeys = 0;
		BGameState->bIsDoorOpen = false;
		BGameState->TimeLimit = 10.0f; //제한시간
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
		&ABGameMode::EndGame,
		BGameState->TimeLimit,
		false
	);

}

void ABGameMode::EnemyDefeated() //적 처치 시 호출
{
	ABGameState* BGameState = GetGameState<ABGameState>();
	if (BGameState)
	{
		BGameState->KilledEnemies++;
	}
	CheckGameStatus();
}

void ABGameMode::ItemCollected() //클리어 조건 아이템 획득 시 호출
{
	ABGameState* BGameState = GetGameState<ABGameState>();
	if (BGameState)
	{
		BGameState->CollectedKeys++;
	}
	CheckGameStatus();
}

void ABGameMode::CheckGameStatus() //게임 클리어 조건 체크
{
	ABGameState* BGameState = GetGameState<ABGameState>();
	if (!BGameState) return;
	
	if (BGameState->CollectedKeys >= 3 || BGameState->SpawnedEnemies <= BGameState->KilledEnemies) //키 3개 이상 획득 or 스폰된 적 전부 처치 시
	{
		OpenDoor();
		return;
	}
	
}

void ABGameMode::OpenDoor()
{
	ABGameState* BGameState = GetGameState<ABGameState>();
	if (BGameState)
	{
		BGameState->bIsDoorOpen = true;
	}
}

void ABGameMode::onDoorReached() //문에 플레이어 도달 시 호출
{
	ABGameState* BGameState = GetGameState<ABGameState>();
	if (BGameState && BGameState->bIsDoorOpen)
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
	UE_LOG(LogTemp, Log, TEXT("Loading next level"));
	UGameplayStatics::OpenLevel(this, "NextLevelName"); //다음 레벨 이름 수정 必
}

void ABGameMode::EndGame()
{
	UBGameInstance* GameInstance = Cast<UBGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->GetUIManagerInstance()->EnterGameOverScreen();
	}
}

void ABGameMode::RestartGame()
{
	UGameplayStatics::OpenLevel(this, "MainLevel"); // 메인레벨로 가서 메뉴띄우기
}

void ABGameMode::QuitGame()
{
	UWorld* World = GetWorld();
	UKismetSystemLibrary::QuitGame(World, nullptr, EQuitPreference::Quit, false);
}