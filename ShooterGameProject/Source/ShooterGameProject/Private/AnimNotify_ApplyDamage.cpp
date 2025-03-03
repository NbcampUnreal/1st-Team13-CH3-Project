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

    // ���÷�, ������ ó���� BEnemyBase�� Attack() �Լ����� ����Ѵٰ� �����մϴ�.
    if (ABEnemyBase* Enemy = Cast<ABEnemyBase>(Owner))
    {
        if (!Enemy->bIsRanged)
        {
            Enemy->AttackPlayer();
        }
    }
}
