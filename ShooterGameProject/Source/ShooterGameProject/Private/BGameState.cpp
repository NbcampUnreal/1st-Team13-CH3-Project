#include "BGameState.h"
#include "BGameInstance.h"
#include "BUIManager.h"
#include "BShopManagerPawn.h"
#include "Kismet/GameplayStatics.h"


ABGameState::ABGameState()
{
	UE_LOG(LogTemp, Warning, TEXT("ABGameState::ABGameState()"));
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
	UE_LOG(LogTemp, Warning, TEXT("ABGameState::BeginPlay()"));
	Super::BeginPlay();
}

void ABGameState::InitializeGameState()
{
	UE_LOG(LogTemp, Warning, TEXT("InitializeGameState! Door closed"));
	SpawnedEnemies = 10; //ex
	KilledEnemies = 0;
	CollectedKeys = 0;
	bIsDoorOpen = false;
	TimeLimit = 20.0f;
	RequiredKeyCount = 1;
}

void ABGameState::EnemyDefeated()
{
	KilledEnemies++;
	CheckGameStatus();
}

void ABGameState::ItemCollected()
{
	CollectedKeys++;
	UE_LOG(LogTemp, Warning, TEXT("CollectedKey! Key : %d"), CollectedKeys);
	CheckGameStatus();
}

void ABGameState::CheckGameStatus()
{
	UBGameInstance* GameInstance = Cast<UBGameInstance>(GetGameInstance());
	if (GameInstance->GetCurrentStage() == 3)
	{
		if (SpawnedEnemies <= KilledEnemies)
		{
			UE_LOG(LogTemp, Warning, TEXT("OpenDoor!"));
			OpenDoor();
		}
	}
	else
	{
		if (CollectedKeys >= RequiredKeyCount || SpawnedEnemies <= KilledEnemies)
		{
			UE_LOG(LogTemp, Warning, TEXT("OpenDoor!"));
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
		if (ABShopManagerPawn* ShopManagerPawn = Cast<ABShopManagerPawn>(ShopManagerPawns[0]))
		{
			ShopManagerPawn->EnableShop();
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