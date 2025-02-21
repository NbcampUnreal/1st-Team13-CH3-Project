// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BBaseGun.h"
#include "BRifle.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAMEPROJECT_API ABRifle : public ABBaseGun
{
    GENERATED_BODY()

public:
    ABRifle();

    virtual void Attack() override;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    UParticleSystem* ImpactEffect;
    // 🔹 본체 (기본 루트)
    UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Components")
    UStaticMeshComponent* RifleBody;

    // 🔹 탄창
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* Magazine;

    // 🔹 조준경 (옵션)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* Scope;
    
    // 🔹 총기 뒷쪽 몸통
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* Derriere;

    // 🔹 총기 트리거
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* Trigger;

    // 🔹 소염기/총구 (옵션)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* Muzzle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
    USceneComponent* GunMuzzle;


    // 피스톨 기본 데미지
    UPROPERTY(EditAnywhere, Category = "Gun")
    float Damage;

    UPROPERTY(EditAnywhere, Category = "Gun")
    USoundBase* FireSound;
    bool bDebugDraw = true;  // 기본값을 true로 설정하여 디버깅할 때 선이 그려지도록 할 수 있습니다.
};