// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BProjectileBase.h"
#include "ABGrenadeProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAMEPROJECT_API AABGrenadeProjectile : public ABProjectileBase
{
	GENERATED_BODY()

public:
	AABGrenadeProjectile();

protected:
	virtual void BeginPlay() override;

public:
	// 폭발 함수
	void Explode();
	// FireInDirection을 오버라이드 선언
	virtual void FireInDirection(const FVector& ShootDirection) override;
private:
	// 폭발 타이머
	FTimerHandle ExplosionTimerHandle;

	// 폭발 시간 (초)
	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	float ExplosionDelay = 3.0f;

	// 폭발 데미지
	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	float ExplosionDamage = 100.0f;

	// 폭발 범위
	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	float ExplosionRadius = 300.0f;

	// 폭발 이펙트
	UPROPERTY(EditDefaultsOnly, Category = "Grenade")
	UParticleSystem* ExplosionEffect;

	

};
