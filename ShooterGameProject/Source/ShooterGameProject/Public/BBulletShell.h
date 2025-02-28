
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BBulletShell.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API ABBulletShell : public AActor
{
    GENERATED_BODY()

public:
    ABBulletShell();

protected:
    // ✅ AActor의 NotifyHit 함수 원형과 일치하도록 수정
    virtual void NotifyHit(
        UPrimitiveComponent* HitComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        bool bSelfMoved,  // 🔹 추가됨
        FVector HitLocation,
        FVector HitNormal,
        FVector NormalImpulse,
        const FHitResult& Hit
    ) override;

    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* ShellMesh;

    UFUNCTION()
    UStaticMeshComponent* GetShellMesh() { return ShellMesh; }

    // 총기 타입별 탄피 메시 변경
    UFUNCTION()
    void SetShellType(FName ShellType);

    UPROPERTY(EditDefaultsOnly, Category = "Shell")
    UStaticMesh* PistolShellMesh = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Shell")
    UStaticMesh* RifleShellMesh = nullptr;
    UPROPERTY(EditDefaultsOnly, Category = "Shell")
    UStaticMesh* ShotGunShellMesh = nullptr;
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    USoundBase* ShellDropSound = nullptr;
};