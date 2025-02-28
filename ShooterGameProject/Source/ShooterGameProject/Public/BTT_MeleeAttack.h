#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MeleeAttack.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UBTT_MeleeAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
    // ������
    UBTT_MeleeAttack();

protected:
    // ���� �½�ũ�� ����� �� ȣ��
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    // �½�ũ�� ����Ǵ� ���� Tick�� �ʿ��� ��� ���(���� ����)
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
    // ������ �������� ���� ���� üũ�ϱ� ���� ���°�
    bool bIsAttacking = false;
};