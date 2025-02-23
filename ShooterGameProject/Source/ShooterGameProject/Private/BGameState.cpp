#include "BGameState.h"
#include "BGameInstance.h"
#include "BUIManager.h"

ABGameState::ABGameState()
{
	CurrentScore = 0;
	KilledEnemies = 0;
	SpawnedEnemies = 0;
	CollectedKeys = 0;
	bIsDoorOpen = false;
	TimeLimit = 10.0f;
	CurrentStage = 1;
}
void ABGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ABGameState::InitializeGameState()
{
	SpawnedEnemies = 10; //ex
	KilledEnemies = 0;
	CollectedKeys = 0;
	bIsDoorOpen = false;
	TimeLimit = 10.0f;
}

void ABGameState::EnemyDefeated()
{
	KilledEnemies++;
	CheckGameStatus();
}

void ABGameState::ItemCollected()
{
	CollectedKeys++;
	CheckGameStatus();
}

void ABGameState::CheckGameStatus()
{
	if (CurrentStage == 3)
	{
		if (SpawnedEnemies <= KilledEnemies)
		{
			OpenDoor();
		}
	}
	else
	{
		if (CollectedKeys >= 3 || SpawnedEnemies <= KilledEnemies)
		{
			OpenDoor();
		}
	}
}

void ABGameState::OpenDoor()
{
	bIsDoorOpen = true;
}

void ABGameState::TriggerGameOver()
{
	if (UBGameInstance* GameInstance = Cast<UBGameInstance>(GetGameInstance()))
	{
		if (UBUIManager* UIManager = GameInstance->GetUIManagerInstance())
		{
			UIManager->EnterGameOverScreen();
		}
	}
}