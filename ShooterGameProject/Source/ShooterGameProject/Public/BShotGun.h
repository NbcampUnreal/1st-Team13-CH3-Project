#pragma once

#include "CoreMinimal.h"
#include "BBaseGun.h"
#include "BCharacter.h"
#include "BProjectileBase.h"
#include "BShotgun.generated.h"

class ABCharacter;
class USoundBase;

UCLASS()
class SHOOTERGAMEPROJECT_API ABShotgun : public ABBaseGun
{
    GENERATED_BODY()

public:
    ABShotgun();

protected:
    virtual void Attack() override;

    FVector GetCrosshairTarget();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    UParticleSystem* ImpactEffect;

    // 🔹 본체 (기본 루트)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* ShotGunBody;

    // 🔹 소염기/총구 (옵션)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* Muzzle;
   
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    USceneComponent* GunMuzzle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    TSubclassOf<ABProjectileBase> ProjectileClass;
    
    UPROPERTY(EditAnywhere, Category = "Gun")
    int32 ShotPelletCount;

    UPROPERTY(EditAnywhere, Category = "Gun")
    float PelletSpreadAngle = 5.0f;

private:
    UPROPERTY(EditAnywhere, Category = "Gun")
    float Damage;

    float LastFireTime;

    UPROPERTY(EditAnywhere, Category = "Gun")
    USoundBase* FireSound;
};
