// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BProjectileBase.h"
#include "BPistolProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAMEPROJECT_API ABPistolProjectile : public ABProjectileBase
{
    GENERATED_BODY()

public:
    // 생성자
    ABPistolProjectile();

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;
	
};
