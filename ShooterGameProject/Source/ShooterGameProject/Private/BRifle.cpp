// Fill out your copyright notice in the Description page of Project Settings.


#include "BRifle.h"
#include "BCharacter.h"       // BCharacter 포함
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ABRifle::ABRifle()
{
    // 소총 기본 설정
    FireRate = 0.1f; // 예: 초당 10발
    AmmoCount = 30;  // 탄창 30발
    ItemPrice = 200; // 가격 200
    WeaponType = "Rifle";
    Damage = 25.0f;  // 소총 기본 데미지
    // 🔹 본체 (Root Component로 설정)
    RifleBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RifleBody"));
    SetRootComponent(RifleBody);
    FRotator NewRotation(0.0f, 0.0f, -180.0f); // 예: Y축으로 90도 회전
    RifleBody->SetRelativeRotation(NewRotation);
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

    // 🔹 총구 위치 및 방향
    FVector MuzzleLocation = GunMuzzle ? GunMuzzle->GetComponentLocation() : GetActorLocation();
    FVector ShootDirection = OwnerCharacter->GetCameraForwardVector();
    FVector EndTrace = MuzzleLocation + (ShootDirection * 15000.0f);  // 15,000 거리까지 사격

    UE_LOG(LogTemp, Log, TEXT("총구 위치: %s, 발사 방향: %s"), *MuzzleLocation.ToString(), *ShootDirection.ToString());

    // 🔹 라인트레이스 설정
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(OwnerCharacter);

    if (bDebugDraw)
    {
        // 라인 그리기 (발사 경로)
        DrawDebugLine(GetWorld(), MuzzleLocation, EndTrace, FColor::Red, false, 1.0f, 0, 1.0f);
    }

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, MuzzleLocation, EndTrace, ECC_Visibility, Params);

    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            // 데미지 적용
            UGameplayStatics::ApplyDamage(HitActor, Damage, OwnerCharacter->GetController(), this, UDamageType::StaticClass());

            // 피격 이펙트 생성
            if (ImpactEffect)
            {
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.ImpactPoint);
            }

            UE_LOG(LogTemp, Log, TEXT("적을 맞췄습니다: %s"), *HitActor->GetName());
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("발사한 총알이 아무 것도 맞추지 못했습니다."));
    }

    // 🔹 발사 사운드 재생
    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }
}