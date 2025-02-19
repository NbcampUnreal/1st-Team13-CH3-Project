#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BGameState.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API ABGameState : public AGameState
{
	GENERATED_BODY()
public:
	ABGameState();

    UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "GameState")
    int32 CurrentScore;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
    int32 KilledEnemies;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
    int32 SpawnedEnemies;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
    int32 CollectedKeys; //�� ���� ���� ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
    bool bIsDoorOpen; //�� ���� (true�� ���� �������� ����)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
	float RemainingTime; //���� �ð�
    
};
