// Fill out your copyright notice in the Description page of Project Settings.


#include "BRifle.h"
#include "BCharacter.h"       // BCharacter 포함
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
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

    // 🔹 총구 위치 및 방향
    FVector MuzzleLocation = GunMuzzle ? GunMuzzle->GetComponentLocation() : GetActorLocation();
    FVector ShootDirection = OwnerCharacter->GetCameraForwardVector();
    FVector EndTrace = MuzzleLocation + (ShootDirection * 15000.0f);  // 15,000 거리까지 사격
    FRotator MuzzleRotation = OwnerCharacter->GetControlRotation();

    // 🔹 라인트레이스 설정
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(OwnerCharacter);


    // 🔹 총알 스폰
    if (ProjectileClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = OwnerCharacter;
        UE_LOG(LogTemp, Log, TEXT("총알 스폰 시도: %s"), *ProjectileClass->GetName());
        // 총알 생성
        ABProjectileBase* Projectile = GetWorld()->SpawnActor<ABProjectileBase>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
        if (Projectile)
        {
            UE_LOG(LogTemp, Log, TEXT("총알 스폰 성공: %s"), *Projectile->GetName());
            Projectile->FireInDirection(ShootDirection);  // 🔹 총알 발사
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ProjectileClass가 설정되지 않았습니다!"));
    }
    // 🔹 발사 사운드 재생
    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }
}