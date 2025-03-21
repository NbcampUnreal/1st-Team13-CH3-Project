// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BBaseWeapon.h"
#include "BGrenadeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAMEPROJECT_API ABGrenadeWeapon : public ABBaseWeapon
{
    GENERATED_BODY()

public:
    ABGrenadeWeapon();
    void ActivateItem(AActor* Activator) override;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* GrenadeBody;
    // 공격 (수류탄 투척)
    virtual void Attack() override;
    void ResetGrenadeThrow();
    bool bCanThrowGrenade;
    // ABGrenadeWeapon 클래스에 추가할 플래그
    bool bHasPickedUpGrenade = false;
    FTimerHandle GrenadeThrowTimerHandle;
protected:
    // 수류탄 투척 프로젝타일 클래스 (BP에서 설정 가능)
    UPROPERTY(EditDefaultsOnly, Category = "Grenade")
    TSubclassOf<class AABGrenadeProjectile> GrenadeClass;
    UPROPERTY(EditDefaultsOnly, Category = "Grenade")
    UTexture2D* ItemIcon;
    // 투척 힘
    UPROPERTY(EditDefaultsOnly, Category = "Grenade")
    float ThrowStrength = 1000.0f;
};