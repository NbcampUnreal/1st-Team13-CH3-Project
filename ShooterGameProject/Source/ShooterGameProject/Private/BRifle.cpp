// Fill out your copyright notice in the Description page of Project Settings.


#include "BRifle.h"
#include "BCharacter.h"       // BCharacter 포함
#include "BUIManager.h"
#include "BGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"


ABRifle::ABRifle()
{
    // 소총 기본 설정
    FireRate = 0.1f; // 예: 초당 10발
    AmmoCount = 30;  // 탄창 30발
    ItemPrice = 200; // 가격 200
    WeaponName = "AK47";
    WeaponType = "Rifle";
    Damage = 25.0f;  // 소총 기본 데미지
    // 🔹 본체 (Root Component로 설정)
    RifleBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RifleBody"));
    SetRootComponent(RifleBody);
    FRotator NewRotation(0.0f, 0.0f, -180.0f); // 예: Y축으로 90도 회전
    RifleBody->SetRelativeRotation(NewRotation);

    // 루트 컴포넌트로 설정
    Collision->SetupAttachment(RifleBody);
    // 🔹 탄창
    Magazine = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Magazine"));
    Magazine->SetupAttachment(RifleBody);
    Magazine->SetRelativeLocation(FVector(0.0f, -5.0f, -10.0f));  // 위치 조정

    // 🔹 조준경 (옵션)
    Scope = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Scope"));
    Scope->SetupAttachment(RifleBody);
    Scope->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));  // 위치 조정

    // 🔹 소염기/총구 (옵션)
    Muzzle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Muzzle"));
    Muzzle->SetupAttachment(RifleBody);
    Muzzle->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));  // 위치 조정
    
    Trigger = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trigger"));
    Trigger->SetupAttachment(RifleBody);
    Trigger->SetRelativeLocation(FVector(0.0f, 0.0f, -3.0f));  // 위치 조정
    
    Derriere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Derriere"));
    Derriere->SetupAttachment(RifleBody);
    Derriere->SetRelativeLocation(FVector(0.0f, 0.0f, 1.0f));  // 위치 조정

    // 기본 총구 위치를 설정 (이것은 예시이며, 적절한 값으로 설정할 필요 있음)
    GunMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("GunMuzzle"));
    GunMuzzle->SetupAttachment(RootComponent); // 총구 위치 설정 (각 총기마다 다를 수 있음)
}

void ABRifle::Attack()
{
    if (!OwnerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("무기 소유 캐릭터가 없습니다!"));
        return;
    }

    if (AmmoCount <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("탄약이 없습니다! 재장전 필요"));
        return;
    }

    // 🔹 탄약 1발 소모
    AmmoCount--;

    // 🔹 총구 위치 가져오기
    FVector MuzzleLocation = GunMuzzle ? GunMuzzle->GetComponentLocation() : GetActorLocation();

    // 🔹 크로스헤어 방향 가져오기
    UBGameInstance* GameInstance = Cast<UBGameInstance>(GetWorld()->GetGameInstance());
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("GameInstance를 찾을 수 없습니다!"));
        return;
    }

    UBUIManager* UIManager = GameInstance->GetUIManagerInstance();
    if (!UIManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("UBUIManager를 찾을 수 없습니다!"));
        return;
    }

    TTuple<FVector, FVector> CrosshairData = UIManager->GetCrosshairLocationAndDirection();
    FVector CrosshairLocation = CrosshairData.Get<0>();
    FVector CrosshairDirection = CrosshairData.Get<1>();

    // 🔹 크로스헤어 보정 (조금 더 먼 위치로 설정)
    FVector AdjustedCrosshairLocation = CrosshairLocation + (CrosshairDirection * 150.0f);
    FVector AdjustedEndTrace = AdjustedCrosshairLocation + (CrosshairDirection * 30000.0f);

    // 🔹 로그 확인
    float Distance = FVector::Dist(MuzzleLocation, AdjustedCrosshairLocation);
    UE_LOG(LogTemp, Log, TEXT("MuzzleLocation: %s"), *MuzzleLocation.ToString());
    UE_LOG(LogTemp, Log, TEXT("CrosshairLocation: %s"), *CrosshairLocation.ToString());
    UE_LOG(LogTemp, Log, TEXT("보정된 CrosshairLocation: %s"), *AdjustedCrosshairLocation.ToString());
    UE_LOG(LogTemp, Log, TEXT("총구-크로스헤어 거리: %f"), Distance);

    // 🔹 라인트레이스 설정
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(OwnerCharacter);

    // 🔹 라인트레이스 실행하여 크로스헤어 방향의 충돌지점 확인
    if (GetWorld()->LineTraceSingleByChannel(HitResult, AdjustedCrosshairLocation, AdjustedEndTrace, ECC_Visibility, Params))
    {
        AdjustedEndTrace = HitResult.ImpactPoint;
        UE_LOG(LogTemp, Log, TEXT("크로스헤어에 맞음, 충돌 지점: %s"), *HitResult.ImpactPoint.ToString());
    }

    // 🔹 총구에서 크로스헤어로 향하는 방향 벡터
    FVector InitialShootDirection = (AdjustedEndTrace - MuzzleLocation).GetSafeNormal();

    // 🔹 크로스헤어에서 최종 직선 방향 벡터
    FVector FinalShootDirection = CrosshairDirection.GetSafeNormal();

    // 🔹 로그 출력
    UE_LOG(LogTemp, Log, TEXT("=== 발사 방향 디버깅 ==="));
    UE_LOG(LogTemp, Log, TEXT("크로스헤어 충돌 지점: %s"), *AdjustedEndTrace.ToString());
    UE_LOG(LogTemp, Log, TEXT("총구에서 크로스헤어 방향(초기): %s"), *InitialShootDirection.ToString());
    UE_LOG(LogTemp, Log, TEXT("크로스헤어 기준 최종 발사 방향: %s"), *FinalShootDirection.ToString());
    UE_LOG(LogTemp, Log, TEXT("크로스헤어 방향 벡터: %s"), *CrosshairDirection.ToString());

    // 🔹 총알 스폰
    if (ProjectileClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = OwnerCharacter;

        UE_LOG(LogTemp, Log, TEXT("총알 스폰 시도: %s"), *ProjectileClass->GetName());

        // 🔹 총구에서 크로스헤어로 향하는 총알 생성
        FVector AdjustedShootDirection = (AdjustedEndTrace - MuzzleLocation).GetSafeNormal();

        ABProjectileBase* Projectile = GetWorld()->SpawnActor<ABProjectileBase>(
            ProjectileClass, MuzzleLocation,
            FRotationMatrix::MakeFromX(AdjustedShootDirection).Rotator(), // 조정된 초기 방향
            SpawnParams
        );

        if (Projectile)
        {
            UE_LOG(LogTemp, Log, TEXT("총알 스폰 성공: %s"), *Projectile->GetName());
            Projectile->FireInDirection(AdjustedShootDirection); // 🔹 조정된 방향으로 발사
        }

    }
}

