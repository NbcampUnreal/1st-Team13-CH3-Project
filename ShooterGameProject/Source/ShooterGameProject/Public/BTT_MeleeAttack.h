#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MeleeAttack.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UBTT_MeleeAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
    // 생성자
    UBTT_MeleeAttack();

protected:
    // 실제 태스크가 실행될 때 호출
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    // 태스크가 실행되는 동안 Tick이 필요한 경우 사용(선택 사항)
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
    // 공격이 끝났는지 여부 등을 체크하기 위한 상태값
    bool bIsAttacking = false;
};