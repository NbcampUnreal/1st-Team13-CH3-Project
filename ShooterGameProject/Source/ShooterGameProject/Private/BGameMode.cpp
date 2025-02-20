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
		BGameState->SpawnedEnemies = 10; //������ �� ��
		BGameState->KilledEnemies = 0;
		BGameState->CollectedKeys = 0;
		BGameState->bIsDoorOpen = false;
		BGameState->TimeLimit = 10.0f; //���ѽð�
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

void ABGameMode::EnemyDefeated() //�� óġ �� ȣ��
{
	ABGameState* BGameState = GetGameState<ABGameState>();
	if (BGameState)
	{
		BGameState->KilledEnemies++;
	}
	CheckGameStatus();
}

void ABGameMode::ItemCollected() //Ŭ���� ���� ������ ȹ�� �� ȣ��
{
	ABGameState* BGameState = GetGameState<ABGameState>();
	if (BGameState)
	{
		BGameState->CollectedKeys++;
	}
	CheckGameStatus();
}

void ABGameMode::CheckGameStatus() //���� Ŭ���� ���� üũ
{
	ABGameState* BGameState = GetGameState<ABGameState>();
	if (!BGameState) return;
	
	if (BGameState->CollectedKeys >= 3 || BGameState->SpawnedEnemies <= BGameState->KilledEnemies) //Ű 3�� �̻� ȹ�� or ������ �� ���� óġ ��
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

void ABGameMode::onDoorReached() //���� �÷��̾� ���� �� ȣ��
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

void ABGameMode::NextLevel() //���� ������ �̵�
{
	UE_LOG(LogTemp, Log, TEXT("Loading next level"));
	UGameplayStatics::OpenLevel(this, "NextLevelName"); //���� ���� �̸� ���� ��
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
	UGameplayStatics::OpenLevel(this, "MainLevel"); // ���η����� ���� �޴�����
}

void ABGameMode::QuitGame()
{
	UWorld* World = GetWorld();
	UKismetSystemLibrary::QuitGame(World, nullptr, EQuitPreference::Quit, false);
}