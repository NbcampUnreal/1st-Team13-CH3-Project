// Fill out your copyright notice in the Description page of Project Settings.


#include "BRifle.h"
#include "BCharacter.h"       // BCharacter 포함
#include "BUIManager.h"
#include "NiagaraComponent.h"  // 🔹 UNiagaraComponent 정의 포함
#include "NiagaraFunctionLibrary.h"
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
    FRotator NewRotation(0.0f, 0.0f, 0.0f); // 예: Y축으로 90도 회전
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

    //총기 배출구 생성
    ShellEjectSocket = CreateDefaultSubobject<USceneComponent>(TEXT("ShellEjectSocket"));
    ShellEjectSocket->SetupAttachment(RootComponent);  // 루트 컴포넌트에 부착
}

void ABRifle::Attack()
{
    if (!OwnerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("무기 소유 캐릭터가 없습니다!"));
        return;
    }

    // 🔹 FireRate 적용 (발사 간격 체크)
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime - LastFireTime < FireRate) // 발사 간격 체크
    {
        UE_LOG(LogTemp, Warning, TEXT("발사 간격이 부족합니다!"));
        return;
    }

    LastFireTime = CurrentTime; // 마지막 발사 시간 갱신
    UE_LOG(LogTemp, Warning, TEXT("⏳ [ABShotgun] 현재 시간: %f, 마지막 발사 시간: %f, FireRate: %f"),
        CurrentTime, LastFireTime, FireRate);
    if (AmmoCount <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("탄약이 없습니다! 재장전 필요"));
        return;
    }

    // 🔹 탄약 1발 소모
    AmmoCount--;

    // 🔹 총구 위치 가져오기
    FVector MuzzleLocation = GunMuzzle ? GunMuzzle->GetComponentLocation() : GetActorLocation();
    // 🔹 머즐 플래시 효과 재생
    if (MuzzleFlashEffect)
    {
        FVector MuzzleFlashLocation = GunMuzzle ? GunMuzzle->GetComponentLocation()
            + (GetActorForwardVector()) // 🔹 총구에서 약간 앞으로 조정
            : GetActorLocation();
        FRotator MuzzleFlashRotation = OwnerCharacter->GetControlRotation(); // 🔹 카메라 방향을 바라보도록 설정

        UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
            MuzzleFlashEffect,
            RifleBody,
            TEXT("GunMuzzle"), // 🔹 소켓 이름
            GunMuzzle ? GunMuzzle->GetRelativeLocation() : FVector::ZeroVector, // 🔹 총구 위치 반영
            GunMuzzle ? GunMuzzle->GetRelativeRotation() : FRotator::ZeroRotator, // 🔹 총구 회전 반영
            EAttachLocation::KeepRelativeOffset, // 🔹 상대 위치 유지
            true
        );

        if (NiagaraComp)
        {
            // 나이아가라 파티클의 'Lifetime'을 설정
            NiagaraComp->SetFloatParameter(TEXT("Lifetime"), 0.001f); // 0.05초 지속
        }
    }
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

    // 🔹 라인트레이스 설정
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(OwnerCharacter);

    // 🔹 라인트레이스 실행하여 크로스헤어 방향의 충돌지점 확인
    if (GetWorld()->LineTraceSingleByChannel(HitResult, AdjustedCrosshairLocation, AdjustedEndTrace, ECC_Visibility, Params))
    {
        AdjustedEndTrace = HitResult.ImpactPoint;
    }

    // 🔹 총구에서 크로스헤어로 향하는 방향 벡터
    FVector AdjustedShootDirection = (AdjustedEndTrace - MuzzleLocation).GetSafeNormal();

    // 🔹 총알 스폰
    if (ProjectileClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = OwnerCharacter;
        // 개별 탄환 데미지 설정
        ABProjectileBase* Projectile = GetWorld()->SpawnActor<ABProjectileBase>(
            ProjectileClass, MuzzleLocation,
            FRotationMatrix::MakeFromX(AdjustedShootDirection).Rotator(),
            SpawnParams
        );

        if (Projectile)
        {
            Projectile->FireInDirection(AdjustedShootDirection);
            Projectile->SetDamage(Damage);
        }
    }

    if (ShellClass)
    {
        FVector ShellEjectLocation = ShellEjectSocket
            ? ShellEjectSocket->GetComponentLocation()
            + (GetActorForwardVector() * -45.0f)  // 🔹 총기 뒷부분으로 이동
            + (GetActorRightVector() * 5.0f)      // 🔹 오른쪽으로 이동
            + (GetActorUpVector() * 2.0f)         // 🔹 살짝 위로 조정
            : GetActorLocation();

        FRotator ShellEjectRotation = ShellEjectSocket
            ? ShellEjectSocket->GetComponentRotation()
            : FRotator::ZeroRotator;

        UE_LOG(LogTemp, Log, TEXT("탄피 스폰 시도: %s"), *ShellClass->GetName());

        ABBulletShell* Shell = GetWorld()->SpawnActor<ABBulletShell>(ShellClass, ShellEjectLocation, ShellEjectRotation);

        if (Shell)
        {
            UE_LOG(LogTemp, Log, TEXT("탄피 스폰 성공: %s"), *Shell->GetName());
            Shell->SetShellType("Rifle");

            FVector EjectDirection =
                (GetActorRightVector() * FMath::RandRange(3.0f, 4.0f)) +  // 🔹 오른쪽으로 더 강하게 튀게
                (GetActorUpVector() * FMath::RandRange(1.5f, 2.0f)) +    // 🔹 위로 더 튀게
                (GetActorForwardVector() * FMath::RandRange(-1.0f, -2.0f)); // 🔹 약간 뒤로 밀려나게


            Shell->GetShellMesh()->AddImpulse(EjectDirection * 15.0f); // 🔹 Impulse 값을 낮춰 자연스럽게 낙하
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("탄피 스폰 실패!"));
        }
    }

    // 🔹 사운드 재생
    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }
    
}


