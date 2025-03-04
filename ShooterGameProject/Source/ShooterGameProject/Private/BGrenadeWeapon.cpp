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

    // 🔹 본체 (Root Component로 설정)
    GrenadeBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeBody"));
    SetRootComponent(GrenadeBody);
    FRotator NewRotation(0.0f, 0.0f, 0.0f); // 예: Y축으로 90도 회전
    GrenadeBody->SetRelativeRotation(NewRotation);
}

void ABGrenadeWeapon::ActivateItem(AActor* Activator)
{
    if (Activator && Activator->ActorHasTag("Player"))
    {
        ABCharacter* ActiveCharacter = Cast<ABCharacter>(Activator);
        if (ActiveCharacter)
        {
            ActiveCharacter->PickupWeapon(this);
            ActiveCharacter->GrenadeCount++;
            UE_LOG(LogTemp, Log, TEXT("Picked up %s and attached to %s"), *GetName(), *ActiveCharacter->GetName());
        }
    }
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
