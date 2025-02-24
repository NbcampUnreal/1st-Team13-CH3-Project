#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"  // ✅ 콜리전 추가
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
};