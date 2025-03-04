// Fill out your copyright notice in the Description page of Project Settings.


#include "BGrenadeWeapon.h"
#include "GameFramework/Actor.h"
#include "BCharacter.h"
#include "ABGrenadeProjectile.h"
#include "Kismet/GameplayStatics.h"

ABGrenadeWeapon::ABGrenadeWeapon()
{
    WeaponType = "Grenade";
    WeaponDamage = 200.0f;
    ItemPrice = 200;

    // 🔹 무기 메쉬를 루트로 설정
    GrenadeBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeBody"));
    SetRootComponent(GrenadeBody);

    // 🔹 부모 클래스에서 생성된 Collision이 WeaponMesh에 부착되었는지 확인
    if (Collision)
    {
        Collision->SetupAttachment(WeaponMesh); // ✅ WeaponMesh에 부착
        Collision->SetSimulatePhysics(false);   // ✅ 물리 시뮬레이션 해제
        Collision->SetRelativeLocation(FVector::ZeroVector); // ✅ 위치 초기화
    }
}


void ABGrenadeWeapon::Attack()
{
    if (!GrenadeClass || !OwnerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("GrenadeClass is NULL or OwnerCharacter is NULL!"));
        return;
    }
    if (OwnerCharacter->GrenadeCount <= 0) 
    {
        UE_LOG(LogTemp, Error, TEXT("GrenadeCount is 0"));
        return;
    }
    FVector SpawnLocation = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * 100.0f;
    FRotator SpawnRotation = OwnerCharacter->GetActorRotation();

    UE_LOG(LogTemp, Warning, TEXT("Spawning Grenade at: %s"), *SpawnLocation.ToString());
    OwnerCharacter -> GrenadeCount--;
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

void ABGrenadeWeapon::ActivateItem(AActor* Activator)
{
    // 오버랩 됐을 때 인벤토리에 아이템 추가(퀵슬롯으로)
    if (Activator && Activator->ActorHasTag("Player"))
    {
        //GrenadeCount를 더해주고 해당 액터 삭제 / 캐릭터에 수류탄 카운트가 0이면 사용안되게 만들기.
        ABCharacter* OverlappingCharacter = Cast<ABCharacter>(Activator);
        OverlappingCharacter->GrenadeCount++;
        UE_LOG(LogTemp, Warning, TEXT("GrenadeCount: %d"), OverlappingCharacter->GrenadeCount);
    }
}
