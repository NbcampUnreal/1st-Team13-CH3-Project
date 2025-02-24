#include "BTT_MeleeAttack.h"
#include "BEnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BEnemyAIController.h"
#include "GameFramework/Actor.h"

UBTT_MeleeAttack::UBTT_MeleeAttack()
{
	NodeName = TEXT("Melee Attack");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    // 1) AI Controller ��������
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon)
    {
        return EBTNodeResult::Failed;
    }

    // 2) ���� AI ĳ���� ��������
    ABEnemyBase* AICharacter = Cast<ABEnemyBase>(AICon->GetPawn());
    if (!AICharacter)
    {
        return EBTNodeResult::Failed;
    }

    // 3) �����忡�� Ÿ�� ��������
    UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetKeyName"));
    AActor* TargetActor = Cast<AActor>(TargetObject);

    if (!TargetActor)
    {
        return EBTNodeResult::Failed;
    }

    // 4) �Ÿ� üũ
    float Distance = AICharacter->GetDistanceTo(TargetActor);
    if (Distance > AICharacter->GetAttackRange())
    {
        // ��Ÿ� ���̸� ���� ó�� or �ٸ� ����
        return EBTNodeResult::Failed;
    }

    // 5) �ٰŸ� ���� ���� (�ִϸ��̼� ��� ��)
    AICharacter->PlayMeleeAttackMontage();
    // -> ���⼭ Animation Montage ���
    // -> AnimNotify�� Montage ����ƿ� ������ �������� ������ ó��

    // ���� �� ���·� ����
    bIsAttacking = true;

    // �����̺�� Ʈ������ �ϴ� "In Progress"�� ��ȯ�ϰ�
    // TickTask���� �ִϸ��̼� �ϷḦ ������ �� "Succeeded"�� �ѱ�� ����
    return EBTNodeResult::InProgress;
}

void UBTT_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    // ������ �̹� �����ٸ� �½�ũ�� ����
    // ������ AnimNotify ��� AICharacter�� "���� �Ϸ�" ��ȣ�� �� �� ����
    // ��: AICharacter->OnAttackEnd ��������Ʈ ��
    // ���⼭�� �ܼ� ���÷� bIsAttacking�� false�� ����� ���� ����

    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    ABEnemyBase* AICharacter = Cast<ABEnemyBase>(AICon->GetPawn());
    if (!AICharacter)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // ��: AICharacter ���ο��� ������ ������ bIsMeleeAttacking = false; �� �����Ѵٰ� ����
    // Ȥ�� AnimMontage ����ƿ� ������ �̺�Ʈ ���ε����� "���� ����"�� AICharacter�� �˸��� ��
    if (!AICharacter->IsMeleeAttacking())
    {
        // ���� �� -> �½�ũ ����
        bIsAttacking = false;
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}