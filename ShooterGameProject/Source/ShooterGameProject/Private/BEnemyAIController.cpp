#include "BEnemyAIController.h"
#include "BEnemyBase.h"
#include "Navigation/PathFollowingComponent.h"

void ABEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//�������� �� �ʱ�ȭ �۾��̳�, �غ� �۾��� ������ ��
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
		ArriveRange,//���� �Ǵ� ���� �Ÿ�(�� ��Ÿ��� �����ϸ� ������)
		true,//�����ϸ� ���� ���ΰ�?
		true,//��ֹ��� ���ذ� ���ΰ�?
		true,//�¿��̵� ���� �� ���ΰ�?
		nullptr,//�̵� ��ο� ���͸� ����
		true//��ü ��θ� ã�� ������ �� �κ� ��ζ� ã�ư� ���ΰ�?
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
	// ���� ���� ���� �� Ÿ�̹� �Ǵ� ��, Pawn�� ���� ���� ��� ����
	if (GetPawn())
	{
		// ���� ���, Pawn�� Attack() �Լ��� ȣ��
		Cast<ABEnemyBase>(GetPawn())->Attack();
	}
}