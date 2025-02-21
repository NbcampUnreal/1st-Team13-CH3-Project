#include "BPistol.h"
#include "BCharacter.h"       // BCharacter 포함
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ABPistol::ABPistol()
{
    FireRate = 0.3f;  // 피스톨 연사 속도 설정
    LastFireTime = -FireRate; // 🔹 시작 시 즉시 발사 가능하도록 설정
    Damage = 20.0f;   // 피스톨 기본 데미지 설정
    // 탄약 기본값 설정
    AmmoCount = 90;
    WeaponName = "M1911";
    WeaponType = "Pistol";
    // 스태틱 메시 컴포넌트 초기화
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;  // RootComponent로 설정
    FRotator NewRotation(0.0f, 90.0f, 90.0f); // 예: Y축으로 90도 회전
    WeaponMesh->SetRelativeRotation(NewRotation);
    // 기본 총구 위치를 설정 (이것은 예시이며, 적절한 값으로 설정할 필요 있음)
    GunMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("GunMuzzle"));
    GunMuzzle->SetupAttachment(RootComponent); // 총구 위치 설정 (각 총기마다 다를 수 있음)
    

    // 물리 시뮬레이션 끄기
    DisablePhysicsSimulation();
}

void ABPistol::Attack()
{
    if (!OwnerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("무기 소유 캐릭터가 없습니다!"));
        return;
    }

    float CurrentTime = GetWorld()->GetTimeSeconds();

    // 🔹 FireRate 체크 (연속 발사 방지)
    if (CurrentTime - LastFireTime < FireRate)
    {
        UE_LOG(LogTemp, Warning, TEXT("발사 대기 중..."));
        return;
    }

    LastFireTime = CurrentTime; // 🔹 마지막 발사 시간 업데이트

    if (AmmoCount <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("탄약이 없습니다! 재장전 필요"));
        return;
    }

    AmmoCount--;

    FVector MuzzleLocation = GunMuzzle ? GunMuzzle->GetComponentLocation() : GetActorLocation();
    FVector ShootDirection = OwnerCharacter->GetCameraForwardVector();
    FVector EndTrace = MuzzleLocation + (ShootDirection * 10000.0f);

    UE_LOG(LogTemp, Log, TEXT("총구 위치: %s, 발사 방향: %s"), *MuzzleLocation.ToString(), *ShootDirection.ToString());

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(OwnerCharacter);

    if (bDebugDraw)
    {
        DrawDebugLine(GetWorld(), MuzzleLocation, EndTrace, FColor::Green, false, 2.0f, 0, 1.0f);
    }

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, MuzzleLocation, EndTrace, ECC_Visibility, Params);

    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            UGameplayStatics::ApplyDamage(HitActor, Damage, OwnerCharacter->GetController(), this, UDamageType::StaticClass());

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

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }
}



void ABPistol::DisablePhysicsSimulation()
{
    if (WeaponMesh)
    {
        WeaponMesh->SetSimulatePhysics(false);
    }
}
