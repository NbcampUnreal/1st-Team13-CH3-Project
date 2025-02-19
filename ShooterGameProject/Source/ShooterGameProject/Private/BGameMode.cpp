#include "BGameMode.h"
#include "BGameState.h"
#include "BPlayerController.h"
#include "BCharacter.h"
#include "Kismet/GameplayStatics.h"

ABGameMode::ABGameMode()
{
	DefaultPawnClass = ABCharacter::StaticClass();
	PlayerControllerClass = ABPlayerController::StaticClass();
	GameStateClass = ABGameState::StaticClass();
}

void ABGameMode::StartGame() //게임시작하면서 state들 초기화
{
    ABGameState* BGameState = GetGameState<ABGameState>();
    if (BGameState)
    {
        //초기화 시킬 state들

    }

    UE_LOG(LogTemp, Log, TEXT("Level Started!"));
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
	if (BGameState->RemainingTime <= 0.0f) //시간 종료 시
	{
		EndGame(false);//게임 오버
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
		NextLevel();
	}
}

void ABGameMode::NextLevel() //다음 레벨로 이동
{
	UE_LOG(LogTemp, Log, TEXT("Loading next level"));
	UGameplayStatics::OpenLevel(this, "NextLevelName"); //다음 레벨 이름 수정 必
}

void ABGameMode::EndGame(bool bIsVictory)
{
	if (bIsVictory)
	{
		UE_LOG(LogTemp, Log, TEXT("Game Clear!")); //게임클리어? 수정必
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Game Over! You Lose!")); // 게임오버
		RestartGame();
	}
}

void ABGameMode::RestartGame()
{
	UGameplayStatics::OpenLevel(this, "RestartMenu"); //restart메뉴==레벨 이동?
}