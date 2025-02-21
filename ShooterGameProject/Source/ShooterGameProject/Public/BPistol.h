#pragma once

#include "CoreMinimal.h"
#include "BBaseGun.h"
#include "BPistol.generated.h"

class ABCharacter;  // ABCharacter 전방 선언
class USoundBase;   // 총소리용 사운드 클래스

UCLASS()
class SHOOTERGAMEPROJECT_API ABPistol : public ABBaseGun
{
    GENERATED_BODY()

public:
    ABPistol();

protected:
    virtual void Attack() override;  // 피스톨 전용 공격 함수

    void DisablePhysicsSimulation() override;
    // 발사 시 사용할 이펙트 (총격 시 피격 이펙트)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    UParticleSystem* ImpactEffect;
    // 무기 메시 (스태틱 메시로 설정)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* WeaponMesh;
    // 총구 위치 (총기마다 다를 수 있으므로 여기서 설정)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    USceneComponent* GunMuzzle;
    // 디버그 선 그리기 여부를 결정하는 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bDebugDraw = true;  // 기본값을 true로 설정하여 디버깅할 때 선이 그려지도록 할 수 있습니다.
private:
   
    // 피스톨 연사 속도
    UPROPERTY(EditAnywhere, Category = "Weapon")
    float FireRate;

    // 피스톨 기본 데미지
    UPROPERTY(EditAnywhere, Category = "Weapon")
    float Damage;
    
    float LastFireTime; // 🔹 마지막으로 발사한 시간 저장
    // 총 발사 소리
    UPROPERTY(EditAnywhere, Category = "Weapon")
    USoundBase* FireSound;
};
