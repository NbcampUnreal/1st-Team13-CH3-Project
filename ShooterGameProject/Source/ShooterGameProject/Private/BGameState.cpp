#include "BGameState.h"
#include "BGameInstance.h"
#include "BUIManager.h"
#include "BShopManagerPawn.h"
#include "Kismet/GameplayStatics.h"


ABGameState::ABGameState()
{
	CurrentScore = 0;
	KilledEnemies = 0;
	SpawnedEnemies = 0;
	CollectedKeys = 0;
	bIsDoorOpen = false;
	TimeLimit = 10.0f;
	RequiredKeyCount = 0;
}
void ABGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ABGameState::InitializeGameState()
{
	UBGameInstance* BGameInstance = GetGameInstance<UBGameInstance>();
	int32 currnetstage=BGameInstance->GetCurrentStage();
	SpawnedEnemies = 14;
	if (currnetstage == 2)
	{
		SpawnedEnemies = 30;
	}
	KilledEnemies = 0;
	CollectedKeys = 0;
	bIsDoorOpen = false;
	TimeLimit = 30.0f;
	RequiredKeyCount = 1;
	//UI Update
	BGameInstance->GetUIManagerInstance()->UpdateHUDItemMission("Battery", CollectedKeys, RequiredKeyCount);
	BGameInstance->GetUIManagerInstance()->UpdateHUDBonusMission(KilledEnemies, SpawnedEnemies);
}

void ABGameState::EnemyDefeated()
{
	KilledEnemies++;
	if (UBGameInstance* GameInstance = GetGameInstance<UBGameInstance>())
	{
		if (UBUIManager* UIManager = GameInstance->GetUIManagerInstance())
		{
			UIManager->UpdateKillCount(KilledEnemies);
			UIManager->UpdateHUDBonusMission(KilledEnemies, SpawnedEnemies);
		}
	}
	CheckGameStatus();
}

void ABGameState::ItemCollected()
{
	CollectedKeys++;
	UBGameInstance* BGameInstance = GetGameInstance<UBGameInstance>();
	BGameInstance->GetUIManagerInstance()->UpdateHUDItemMission("Battery", CollectedKeys, RequiredKeyCount);
	CheckGameStatus();
}

void ABGameState::CheckGameStatus()
{
	UBGameInstance* GameInstance = Cast<UBGameInstance>(GetGameInstance());
	if (GameInstance->GetCurrentStage() == 3)
	{
		if (SpawnedEnemies <= KilledEnemies || CollectedKeys >= RequiredKeyCount)//test ������ || CollectedKeys >= RequiredKeyCount ����
		{
			UE_LOG(LogTemp, Warning, TEXT("The door has opened."));
			OpenDoor();
		}
	}
	else
	{
		if (CollectedKeys >= RequiredKeyCount || SpawnedEnemies <= KilledEnemies)
		{
			UE_LOG(LogTemp, Warning, TEXT("The door has opened."));
			OpenDoor();
		}
	}
}

void ABGameState::OpenDoor()
{
	bIsDoorOpen = true;

	TArray<AActor*> ShopManagerPawns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABShopManagerPawn::StaticClass(), ShopManagerPawns);

	if (!ShopManagerPawns.IsEmpty())
	{
		for (int i = 0; i < ShopManagerPawns.Num(); i++)
		{
			if (ABShopManagerPawn* ShopManagerPawn = Cast<ABShopManagerPawn>(ShopManagerPawns[i]))
			{
				ShopManagerPawn->EnableShop();
			}
		}
	}
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