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
}
void ABGameState::BeginPlay()
{
	//if (UBGameInstance* GameInstance = Cast<UBGameInstance>(GetGameInstance()))
	//{
	//	if (UBUIManager* UIManager = Cast<UBUIManager>(GameInstance->GetUIManagerInstance()))
	//	{
	//		UIManager->EnterGameOverScreen();
	//	}
	//}
}