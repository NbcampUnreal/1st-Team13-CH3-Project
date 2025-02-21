// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BProjectileBase.generated.h"


UCLASS()
class SHOOTERGAMEPROJECT_API ABProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:
    // 생성자
    ABProjectileBase();

protected:
    virtual void BeginPlay() override;
    
public:
    virtual void Tick(float DeltaTime) override;

    // 총알의 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    float Speed;
    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    UProjectileMovementComponent* ProjectileMovement;
    // 총알 메시 (시각적 요소)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    UStaticMeshComponent* MeshComponent;
    // 발사 방향 설정 함수 추가
    void FireInDirection(const FVector& ShootDirection);
    // 충돌 시 처리되는 함수
    UFUNCTION()
    void OnHit();

};
