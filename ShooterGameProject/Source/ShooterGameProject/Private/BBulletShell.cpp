// Fill out your copyright notice in the Description page of Project Settings.


#include "BBulletShell.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ABBulletShell::ABBulletShell()
{
    PrimaryActorTick.bCanEverTick = false;

    ShellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShellMesh"));
    RootComponent = ShellMesh;

    // 🔹 물리 적용
    ShellMesh->SetSimulatePhysics(true);
    ShellMesh->SetEnableGravity(true);  // ✅ 중력 적용 (떨어지도록)
    ShellMesh->SetMassOverrideInKg(NAME_None, 5.0f,true);  // ✅ 질량 조정 (더 자연스러운 낙하)
    // ✅ 랜덤 회전 값 조정 (기존 50~150 → 10~40)
    ShellMesh->AddTorqueInDegrees(FVector(FMath::RandRange(10, 40), FMath::RandRange(10, 40), FMath::RandRange(10, 40)));
    // 🔹 충돌 설정
    ShellMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    ShellMesh->SetCollisionResponseToAllChannels(ECR_Block);
    ShellMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
    ShellMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
    ShellMesh->SetPhysicsLinearVelocity(ShellMesh->GetPhysicsLinearVelocity() * 0.5f);
    // 🔹 바닥 충돌 감지 활성화
    ShellMesh->SetNotifyRigidBodyCollision(true);
    ShellMesh->BodyInstance.bUseCCD = true;
    // 🔹 공기 저항과 회전 감속 설정
    ShellMesh->SetLinearDamping(0.3f); // ✅ 공기 저항을 줄여 더 부드러운 이동
    ShellMesh->SetAngularDamping(1.5f); // ✅ 회전이 너무 빨리 멈추지 않도록 감소
}

// 🔹 바닥 충돌 시 이벤트 처리 (사운드, 탄피 멈추기)
void ABBulletShell::NotifyHit(
    UPrimitiveComponent* HitComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    bool bSelfMoved,  // 🔹 추가됨
    FVector HitLocation,
    FVector HitNormal,
    FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (ShellDropSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ShellDropSound, GetActorLocation());
    }

    // ✅ 바닥에 닿으면 움직임 멈추기 (바닥에 쌓이도록)
    ShellMesh->SetSimulatePhysics(false);
}

void ABBulletShell::BeginPlay()
{
    Super::BeginPlay();

    // 랜덤 회전 추가
    ShellMesh->AddTorqueInDegrees(FVector(0, 0, FMath::RandRange(50, 150)));
    ShellMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);  // 🔹 플레이어와 충돌 무시
    // 탄피 바닥 충돌 시 사운드 재생
    if (ShellDropSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ShellDropSound, GetActorLocation(),0.5f,0.5f);
    }
    // 일정 시간 후 탄피 제거
    SetLifeSpan(5.0f);
}

void ABBulletShell::SetShellType(FName ShellType)
{
    if (ShellType == "Pistol" && PistolShellMesh)
    {
        ShellMesh->SetStaticMesh(PistolShellMesh);
    }
    else if (ShellType == "Rifle" && RifleShellMesh)
    {
        ShellMesh->SetStaticMesh(RifleShellMesh);
    }
    else if (ShellType == "ShotGun" && ShotGunShellMesh)
    {
        ShellMesh->SetStaticMesh(ShotGunShellMesh);
    }
}


