// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BBaseGun.h"
#include "BProjectileBase.h"
#include "BBulletShell.h"
#include "BRifle.generated.h"

class UNiagaraSystem; // 머즐 플래시용 나이아가라 시스템

UCLASS()
class SHOOTERGAMEPROJECT_API ABRifle : public ABBaseGun
{
    GENERATED_BODY()

public:
    ABRifle();

    virtual void Attack() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    UParticleSystem* ImpactEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* RifleBody;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* Magazine;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* Scope;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* Derriere;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* Trigger;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* Muzzle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    TSubclassOf<ABProjectileBase> ProjectileClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    USceneComponent* GunMuzzle;
    // 탄피 배출 위치
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    USceneComponent* ShellEjectSocket;
  
    // 머즐 플래시 효과
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    UNiagaraSystem* MuzzleFlashEffect;
    UPROPERTY(EditAnywhere, Category = "Gun")
    float Damage;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<ABBulletShell> ShellClass;  // 🔹 탄피 클래스 추가
    UPROPERTY(EditAnywhere, Category = "Gun")
    USoundBase* FireSound;

    bool bDebugDraw = true;
};