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
    int32 CollectedKeys; //문 여는 열쇠 개수
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
    bool bIsDoorOpen; //문 상태 (true면 다음 스테이지 가능)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
	float RemainingTime; //남은 시간
    
};
