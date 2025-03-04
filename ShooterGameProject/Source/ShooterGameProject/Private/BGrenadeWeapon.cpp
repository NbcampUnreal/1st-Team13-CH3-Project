// Fill out your copyright notice in the Description page of Project Settings.


#include "BGrenadeWeapon.h"
#include "GameFramework/Actor.h"
#include "BCharacter.h"
#include "ABGrenadeProjectile.h"
#include "Kismet/GameplayStatics.h"

ABGrenadeWeapon::ABGrenadeWeapon()
{
    WeaponType = "Grenade";
    WeaponDamage = 200.0f; // 기본 폭발 데미지
}

void ABGrenadeWeapon::Attack()
{
    if (!GrenadeClass || !OwnerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("GrenadeClass is NULL or OwnerCharacter is NULL!"));
        return;
    }

    FVector SpawnLocation = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * 100.0f;
    FRotator SpawnRotation = OwnerCharacter->GetActorRotation();

    UE_LOG(LogTemp, Warning, TEXT("Spawning Grenade at: %s"), *SpawnLocation.ToString());

    AABGrenadeProjectile* Grenade = GetWorld()->SpawnActor<AABGrenadeProjectile>(
        GrenadeClass, SpawnLocation, SpawnRotation);

    if (!Grenade)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn Grenade!"));
        return;
    }

    // ✅ 충돌 활성화 후 속도 설정
    if (Grenade->ProjectileMovement)
    {
        Grenade->ProjectileMovement->Velocity =
            OwnerCharacter->GetActorForwardVector() * ThrowStrength + FVector(0, 0, 300.0f);
    }
}
