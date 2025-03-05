// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"  // ✅ 콜리전 추가
#include "Kismet/GameplayStatics.h"  // ✅ 사운드 재생을 위한 UGameplayStatics 추가
#include "NiagaraSystem.h"  // Niagara 사용을 위한 헤더 추가
#include "ABGrenadeProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAMEPROJECT_API AABGrenadeProjectile : public AActor
{
	GENERATED_BODY()

public:
	AABGrenadeProjectile();

protected:
	virtual void BeginPlay();
	FVector ExplosionLocation = FVector::ZeroVector; // 충돌 위치 저장용 변수

public:
	// FireInDirection을 오버라이드 선언
	void FireInDirection(const FVector& ShootDirection);
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void Explode();
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

private:
	// 폭발 타이머
	FTimerHandle ExplosionTimerHandle;
	
	// 폭발 시간 (초)
	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	float ExplosionDelay = 3.0f;

	// 폭발 데미지
	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	float ExplosionDamage = 200.0f;

	// 폭발 범위
	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	float ExplosionRadius = 300.0f;

	// 폭발 이펙트
	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	UNiagaraSystem* ExplosionEffect;
};
