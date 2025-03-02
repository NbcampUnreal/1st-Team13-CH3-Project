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

	// AIPerception ������Ʈ �� Sight ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY()
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere)
	UBehaviorTree* AIBehaviorTree;
	UPROPERTY(VisibleAnywhere)
	UBlackboardData* BlackboardData;

	// ���� ���� (bIsInBattle)�� ���� AIController���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Battle")
	bool bIsInBattle;
	FTimerHandle RallyTimerHandle;

	// OnPerceptionUpdated �̺�Ʈ �Լ�
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	void UpdateRallyMoveCommand();
	bool IsPlayerOnNavMesh(ABCharacter* Player) const;
};
