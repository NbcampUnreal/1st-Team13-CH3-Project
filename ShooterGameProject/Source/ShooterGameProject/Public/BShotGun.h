#pragma once

#include "CoreMinimal.h"
#include "BBaseGun.h"
#include "BCharacter.h"
#include "BProjectileBase.h"
#include "BBulletShell.h"
#include "BShotgunPart.h"
#include "BShotgun.generated.h"

class ABCharacter;
class USoundBase;
class UNiagaraSystem; // 머즐 플래시용 나이아가라 시스템

UCLASS()
class SHOOTERGAMEPROJECT_API ABShotgun : public ABBaseGun
{
    GENERATED_BODY()

public:
    ABShotgun();
    // 총기에 장착된 파츠의 매쉬
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gun")
    UStaticMeshComponent* EquippedPartMesh;
    // 파츠 장착 여부 확인
    bool IsPartMeshEquipped(ABShotgunPart* Part);
    UPROPERTY(EditAnywhere, Category = "Gun")
    float Damage;
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
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    USceneComponent* ShellEjectSocket;
   
    // 머즐 플래시 효과
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    UNiagaraSystem* MuzzleFlashEffect;
    UPROPERTY(EditAnywhere, Category = "Gun")
    float PelletSpreadAngle = 5.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<ABBulletShell> ShellClass;  // 🔹 탄피 클래스 추가
private:


    float LastFireTime;
    bool bCanFire = true;
    FTimerHandle FireResetTimer;
    FRotator SplitRotation; // 🔹 탄환 분할 시 사용할 기준 회전값
    UPROPERTY(EditAnywhere, Category = "Gun")
    USoundBase* FireSound;
};
