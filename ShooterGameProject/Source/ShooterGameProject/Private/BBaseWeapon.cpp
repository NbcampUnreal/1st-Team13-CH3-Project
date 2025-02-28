// Fill out your copyright notice in the Description page of Project Settings.


#include "BBaseWeapon.h"
#include "BCharacter.h"
ABBaseWeapon::ABBaseWeapon()
{
	ItemType = "Weapon";
    WeaponType = "Melee";
    WeaponDamage = 5;
    // 🔹 WeaponMesh가 없는 경우, 여기서 새로 생성
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    SetRootComponent(WeaponMesh);
}

void ABBaseWeapon::ActivateItem(AActor* Activator)
{
    if (Activator && Activator->ActorHasTag("Player"))
    {
        ABCharacter* ActiveCharacter = Cast<ABCharacter>(Activator);
        if (ActiveCharacter)
        {
            ActiveCharacter->PickupWeapon(this);

            // 🔹 무기를 플레이어에게 붙이기
            USkeletalMeshComponent* CharacterMesh = ActiveCharacter->GetMesh();
            if (CharacterMesh)
            {
                FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
                AttachToComponent(CharacterMesh, AttachRules, TEXT("WeaponSocket"));
                SetOwnerCharacter(ActiveCharacter);
                SetActorEnableCollision(false);
                
            }
            UE_LOG(LogTemp, Log, TEXT("Picked up %s and attached to %s"), *GetName(), *ActiveCharacter->GetName());
        }
    }
}

void ABBaseWeapon::SetOwnerCharacter(ABCharacter* NewOwner)
{
    if (NewOwner)
    {
        OwnerCharacter = NewOwner;
        SetOwner(NewOwner);  // Unreal 기본 Owner 설정 (필요 시)
    }
}

void ABBaseWeapon::EquipWeapon(AActor* NewOwner)
{
    if (!NewOwner) return;

    Character = Cast<ABCharacter>(NewOwner);
    if (Character)
    {
        // 일단 캐릭터에 Weapon 슬롯이 추가되면 추가하면 됨. 캐릭터 개발 담당자에게 이런방식으로 진행된다는걸 알려주기 위해 예시로 추가해놓음
        //// 기존 장착된 무기가 있다면 해제 (드롭, 삭제, 인벤토리로 이동 등)
        //if (PlayerCharacter->CurrentWeapon)
        //{
        //    PlayerCharacter->CurrentWeapon->Destroy(); // 어떻게 될 지 몰라서 디스트로이로 만들어놨지만 인벤토리가 있다면 인벤토리에 추가되게 바꾸면 됨.
        //}

        //// 무기 장착 처리
        //PlayerCharacter->CurrentWeapon = this;

        //// 캐릭터 손(또는 무기 장착 지점)으로 부착
        //AttachToComponent(PlayerCharacter->GetMesh(),
        //    FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        //    FName("WeaponSocket"));

        //// 필요하다면 충돌 비활성화
        //Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void ABBaseWeapon::Attack()
{
    // 해당 클래스를 상속받은 자식 클래스들 에서 만들 예정
    UE_LOG(LogTemp, Log, TEXT("무기 공격 완료!"));
}
void ABBaseWeapon::DisablePhysicsSimulation()
{
    // 자식 클래스에서 오버라이드해서 물리 시뮬레이션 끄기
}