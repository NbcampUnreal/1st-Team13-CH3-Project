#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BEnemyAIController.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API ABEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

protected:
	//int32 CurrentPatrolIndex = 0;

	//void MoveToCurrentPatrolPoint();
	void AttackPlayer();
};
