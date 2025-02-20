#include "BGameState.h"
#include "BGameInstance.h"
#include "BUIManager.h"

void ABGameState::BeginPlay()
{
	if (UBGameInstance* GameInstance = Cast<UBGameInstance>(GetGameInstance()))
	{
		if (UBUIManager* UIManager = Cast<UBUIManager>(GameInstance->GetUIManagerInstance()))
		{
			UIManager->EnterGameOverScreen();
		}
	}
}
