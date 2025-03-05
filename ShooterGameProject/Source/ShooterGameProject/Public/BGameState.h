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
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Game State")
    int32 CurrentScore;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
    int32 KilledEnemies;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
    int32 SpawnedEnemies;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
    int32 CollectedKeys; //얻은 열쇠 개수
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
    bool bIsDoorOpen; //문 상태 (true면 다음 스테이지 가능)
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game State")
    float TimeLimit; //제한시간
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game State")
    int32 RequiredKeyCount; //필요 열쇠 개수

    UFUNCTION(BlueprintCallable)
    void InitializeGameState();
    UFUNCTION(BlueprintCallable)
    void EnemyDefeated();
    UFUNCTION(BlueprintCallable)
    void ItemCollected();
    UFUNCTION(BlueprintCallable)
    void CheckGameStatus();
    UFUNCTION(BlueprintCallable)
    void OpenDoor();
    UFUNCTION(BlueprintCallable)
    void TriggerGameOver();


    int32  GetCurrentScore() const { return CurrentScore; }
};
