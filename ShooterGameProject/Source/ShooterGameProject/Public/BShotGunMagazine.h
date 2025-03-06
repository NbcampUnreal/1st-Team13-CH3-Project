// BShotgunMagazine.h
#pragma once

#include "CoreMinimal.h"
#include "BBaseItem.h"  // Assuming you are inheriting from BItem
#include "BShotgunMagazine.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API ABShotgunMagazine : public ABBaseItem
{
    GENERATED_BODY()

public:
    // 기본 생성자
    ABShotgunMagazine();

    // 탄약 수 (예시: 8발)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    int32 AmmoAmount;

    // 아이템 사용 함수
    UFUNCTION(BlueprintCallable, Category = "Item")
    virtual void UseItem(AActor* Activator) override;
};
