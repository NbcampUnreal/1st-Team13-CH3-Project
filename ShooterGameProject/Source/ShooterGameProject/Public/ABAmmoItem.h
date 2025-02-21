#pragma once

#include "CoreMinimal.h"
#include "BBaseItem.h"
#include "ABAmmoItem.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API ABAmmoItem : public ABBaseItem
{
    GENERATED_BODY()

public:
    ABAmmoItem();

    // 탄약 개수 (아이템 개수처럼 관리)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    int32 AmmoCount;

    // 탄약 타입 (예: 소총, 권총, 샷건 등)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    FName AmmoType;

    // 아이템을 사용할 때(= 재장전할 때) 호출
    virtual void ActivateItem(AActor* Activator) override;
};