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

void ABGameMode::StartGame() //���ӽ����ϸ鼭 state�� �ʱ�ȭ
{
    ABGameState* BGameState = GetGameState<ABGameState>();
    if (BGameState)
    {
        //�ʱ�ȭ ��ų state��

    }

    UE_LOG(LogTemp, Log, TEXT("Level Started!"));
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
	if (BGameState->RemainingTime <= 0.0f) //�ð� ���� ��
	{
		EndGame(false);//���� ����
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
		NextLevel();
	}
}

void ABGameMode::NextLevel() //���� ������ �̵�
{
	UE_LOG(LogTemp, Log, TEXT("Loading next level"));
	UGameplayStatics::OpenLevel(this, "NextLevelName"); //���� ���� �̸� ���� ��
}

void ABGameMode::EndGame(bool bIsVictory)
{
	if (bIsVictory)
	{
		UE_LOG(LogTemp, Log, TEXT("Game Clear!")); //����Ŭ����? ������
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Game Over! You Lose!")); // ���ӿ���
		RestartGame();
	}
}

void ABGameMode::RestartGame()
{
	UGameplayStatics::OpenLevel(this, "RestartMenu"); //restart�޴�==���� �̵�?
}