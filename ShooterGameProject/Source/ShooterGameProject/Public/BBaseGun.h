#pragma once

#include "CoreMinimal.h"
#include "BBaseWeapon.h"
#include "BBaseGun.generated.h"

class ABCharacter; // 캐릭터 클래스 전방 선언
class USceneComponent;  // 총구 위치를 위한 씬 컴포넌트

UCLASS(BlueprintType, Blueprintable)
class SHOOTERGAMEPROJECT_API ABBaseGun : public ABBaseWeapon
{
    GENERATED_BODY()

public:
    ABBaseGun();

    // 탄약 개수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    int32 AmmoCount;

    // 최대 탄약 개수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    int32 MaxAmmo;

    UPROPERTY(VisibleAnywhere, Category = "Ammo")
    int32 CurrentAmmo;  // 현재 장전된 탄약

    UPROPERTY(VisibleAnywhere, Category = "Ammo")
    int32 ReservedAmmo;  // 예비 탄약 (탄창에 남은 총알)


    // 재장전 기능
    void AddAmmo(int32 Amount);
    void Reload();
    // 총기 공격 동작 (발사)
    virtual void Attack() override;

};
