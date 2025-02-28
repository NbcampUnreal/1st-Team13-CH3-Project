#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"  // ✅ 콜리전 추가
#include "Kismet/GameplayStatics.h"  // ✅ 사운드 재생을 위한 UGameplayStatics 추가
#include "Sound/SoundCue.h"  // ✅ SoundCue 타입 사용을 위한 헤더 추가
#include "BProjectileBase.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API ABProjectileBase : public AActor
{
    GENERATED_BODY()

public:
    ABProjectileBase();
    // ✅ 충돌 감지용 컴포넌트
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
    USphereComponent* CollisionComponent;

    // 총알의 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    float Speed;

    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    UProjectileMovementComponent* ProjectileMovement;

    // 총알 메시 (시각적 요소)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    UStaticMeshComponent* MeshComponent;

    // ✅ 충돌 시 실행될 함수
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    virtual void FireInDirection(const FVector& ShootDirection);

    float Damage; // 개별 총알의 데미지

public:
    void SetDamage(float NewDamage) { Damage = NewDamage; }

    // ✅ 표면별 사운드를 위한 Sound Cue
    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundCue* HitSoundCue;
};