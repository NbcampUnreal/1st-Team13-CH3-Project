#include "BEnemyAIController.h"
#include "BEnemyBase.h"
#include "Navigation/PathFollowingComponent.h"

void ABEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//빙의했을 때 초기화 작업이나, 준비 작업을 넣으면 됨
}

void ABEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	MoveToCurrentPatrolPoint();
}

void ABEnemyAIController::MoveToCurrentPatrolPoint()
{
	ABEnemyBase* Enemy0 = Cast<ABEnemyBase>(GetPawn());
	if (!Enemy0)
	{
		return;
	}

	if (Enemy0->PatrolPoints.Num() == 0)
	{
		return;
	}

	float ArriveRange = Enemy0->GetAttackRange();

	MoveToActor(
		Enemy0->PatrolPoints[CurrentPatrolIndex],
		ArriveRange,//도착 판단 기준 거리(적 사거리로 설정하면 좋을듯)
		true,//도착하면 멈출 것인가?
		true,//장애물을 피해갈 것인가?
		true,//좌우이동 없이 갈 것인가?
		nullptr,//이동 경로에 필터를 지정
		true//전체 경로를 찾지 못했을 때 부분 경로라도 찾아갈 것인가?
	);

	CurrentPatrolIndex = (CurrentPatrolIndex + 1) % Enemy0->PatrolPoints.Num();
}

void ABEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (Result.Code == EPathFollowingResult::Success)
	{
		MoveToCurrentPatrolPoint();
	}
}

void ABEnemyAIController::AttackPlayer()
{
	// 공격 실행 여부 및 타이밍 판단 후, Pawn에 실제 공격 명령 전달
	if (GetPawn())
	{
		// 예를 들어, Pawn의 Attack() 함수를 호출
		Cast<ABEnemyBase>(GetPawn())->Attack();
	}
}