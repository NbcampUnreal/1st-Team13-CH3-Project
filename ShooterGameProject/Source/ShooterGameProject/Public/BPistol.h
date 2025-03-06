#pragma once

#include "CoreMinimal.h"
#include "BBaseGun.h"
#include "BProjectileBase.h"
#include "BBulletShell.h"
#include "BPistolPart.h"
#include "BPistol.generated.h"


class ABCharacter;  // ABCharacter 전방 선언
class USoundBase;   // 총소리용 사운드 클래스
class UNiagaraSystem; // 머즐 플래시용 나이아가라 시스템

UCLASS()
class SHOOTERGAMEPROJECT_API ABPistol : public ABBaseGun
{
    GENERATED_BODY()

public:
    ABPistol();
  

    // 총기에 장착된 파츠의 매쉬
        UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pistol")
    UStaticMeshComponent * EquippedPartMesh;
    // 파츠 장착 여부 확인
    bool IsPartMeshEquipped(ABPistolPart* Part);
    UPROPERTY(EditAnywhere, Category = "Gun")
    float Damage;
protected:
    virtual void Attack() override;  // 피스톨 전용 공격 함수

    void DisablePhysicsSimulation() override;

    
    
    // 발사 시 사용할 이펙트 (총격 시 피격 이펙트)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    UParticleSystem* ImpactEffect;

    // 총구 위치 (총기마다 다를 수 있으므로 여기서 설정)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    USceneComponent* GunMuzzle;

    // 탄피 배출 위치
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    USceneComponent* ShellEjectSocket;

    // 머즐 플래시 효과
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    UNiagaraSystem* MuzzleFlashEffect;

    // 디버그 선 그리기 여부를 결정하는 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bDebugDraw = true;  // 기본값을 true로 설정하여 디버깅할 때 선이 그려지도록 할 수 있습니다.

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    TSubclassOf<ABProjectileBase> ProjectileClass;  // 🔹 총알 클래스 추가

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<ABBulletShell> ShellClass;  // 🔹 탄피 클래스 추가
   

    float LastFireTime; // 🔹 마지막으로 발사한 시간 저장
private:
    

    // 총 발사 소리
    UPROPERTY(EditAnywhere, Category = "Gun")
    USoundBase* FireSound;
};
