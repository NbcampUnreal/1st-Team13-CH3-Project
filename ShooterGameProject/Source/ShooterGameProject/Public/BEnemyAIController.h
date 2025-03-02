#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BEnemyAIController.generated.h"

class ABCharacter;

UCLASS()
class SHOOTERGAMEPROJECT_API ABEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABEnemyAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	// AIPerception 컴포넌트 및 Sight 설정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY()
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere)
	UBehaviorTree* AIBehaviorTree;
	UPROPERTY(VisibleAnywhere)
	UBlackboardData* BlackboardData;

	// 전투 상태 (bIsInBattle)는 이제 AIController에서 관리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Battle")
	bool bIsInBattle;
	FTimerHandle RallyTimerHandle;

	// OnPerceptionUpdated 이벤트 함수
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	void UpdateRallyMoveCommand();
	bool IsPlayerOnNavMesh(ABCharacter* Player) const;
};
