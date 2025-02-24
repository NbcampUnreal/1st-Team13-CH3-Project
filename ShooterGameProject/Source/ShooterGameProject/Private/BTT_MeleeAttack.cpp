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

    // 1) AI Controller 가져오기
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon)
    {
        return EBTNodeResult::Failed;
    }

    // 2) 실제 AI 캐릭터 가져오기
    ABEnemyBase* AICharacter = Cast<ABEnemyBase>(AICon->GetPawn());
    if (!AICharacter)
    {
        return EBTNodeResult::Failed;
    }

    // 3) 블랙보드에서 타겟 가져오기
    UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetKeyName"));
    AActor* TargetActor = Cast<AActor>(TargetObject);

    if (!TargetActor)
    {
        return EBTNodeResult::Failed;
    }

    // 4) 거리 체크
    float Distance = AICharacter->GetDistanceTo(TargetActor);
    if (Distance > AICharacter->GetAttackRange())
    {
        // 사거리 밖이면 실패 처리 or 다른 로직
        return EBTNodeResult::Failed;
    }

    // 5) 근거리 공격 시작 (애니메이션 재생 등)
    AICharacter->PlayMeleeAttackMontage();
    // -> 여기서 Animation Montage 재생
    // -> AnimNotify나 Montage 블렌드아웃 시점에 데미지를 실제로 처리

    // 공격 중 상태로 변경
    bIsAttacking = true;

    // 비헤이비어 트리에선 일단 "In Progress"로 반환하고
    // TickTask에서 애니메이션 완료를 감지한 뒤 "Succeeded"로 넘기는 패턴
    return EBTNodeResult::InProgress;
}

void UBTT_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    // 공격이 이미 끝났다면 태스크를 종료
    // 보통은 AnimNotify 등에서 AICharacter가 "공격 완료" 신호를 줄 수 있음
    // 예: AICharacter->OnAttackEnd 델리게이트 등
    // 여기서는 단순 예시로 bIsAttacking을 false로 만드는 순간 종료

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

    // 예: AICharacter 내부에서 공격이 끝나면 bIsMeleeAttacking = false; 로 설정한다고 가정
    // 혹은 AnimMontage 블렌드아웃 시점에 이벤트 바인딩으로 "공격 종료"를 AICharacter에 알리는 식
    if (!AICharacter->IsMeleeAttacking())
    {
        // 공격 끝 -> 태스크 종료
        bIsAttacking = false;
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}