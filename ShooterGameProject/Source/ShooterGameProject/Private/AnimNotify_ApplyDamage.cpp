#include "AnimNotify_ApplyDamage.h"
#include "BEnemyBase.h"
#include "GameFramework/Actor.h"

void UAnimNotify_ApplyDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp)
        return;

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner)
        return;

    // 예시로, 데미지 처리를 BEnemyBase의 Attack() 함수에서 담당한다고 가정합니다.
    if (ABEnemyBase* Enemy = Cast<ABEnemyBase>(Owner))
    {
        if (!Enemy->bIsRanged)
        {
            Enemy->AttackPlayer();
        }
    }
}
