// BShotgunMagazine.cpp
#include "BShotgunMagazine.h"
#include "BBaseGun.h"
#include "BCharacter.h"
#include "BPlayerState.h"

ABShotgunMagazine::ABShotgunMagazine()
{
    ItemName = TEXT("ShotgunMagazine");
    AmmoAmount = 12;  // 샷건에 추가할 탄약 수 (예시: 12발)

    // FItemData 초기화
    FItemData NewItemData;
    NewItemData.ItemName = TEXT("ShotgunMagazine");
    NewItemData.ItemClass = ABShotgunMagazine::StaticClass();
    NewItemData.ItemCount = 1;  // 기본적으로 1개씩 추가
    NewItemData.ItemIcon = nullptr;  // 필요 시 아이콘 설정
    NewItemData.ItemRef = this;

    // SetItemData를 통해 ItemData 설정
    SetItemData(NewItemData);
}

void ABShotgunMagazine::UseItem(AActor* Activator)
{
    if (!Activator) return;

    ABCharacter* Character = Cast<ABCharacter>(Activator);
    if (!Character) return;

    // 현재 장착된 무기를 확인
    if (ABBaseWeapon* CurrentWeapon = Character->EquippedWeapon) // EquippedWeapon으로 현재 장착된 무기 확인
    {
        // 현재 무기가 샷건인지 확인
        if (CurrentWeapon->WeaponType == "Shotgun") // 샷건인지 확인
        {
            if (ABBaseGun* CurrentGun = Cast<ABBaseGun>(CurrentWeapon))
            {
                // 탄약 추가
                CurrentGun->AddAmmo(AmmoAmount);
                UE_LOG(LogTemp, Log, TEXT("샷건 탄창 사용: 예비탄환 %d발 추가"), AmmoAmount);

                // 인벤토리에서 해당 아이템이 몇 개 남았는지 확인
                if (ABPlayerState* PlayerState = Character->GetPlayerState<ABPlayerState>())
                {
                    PlayerState->InventoryRemoveItem(ItemData);  // 인벤토리에서 해당 아이템 제거
                    UE_LOG(LogTemp, Log, TEXT("탄창을 장착 완료해 샷건 탄창 아이템을 제거했습니다"));

                    // 인벤토리에서 아이템의 남은 개수를 출력
                    TArray<FItemData> AmmoItems = PlayerState->GetInventoryTypeItem(ItemData.ItemName);
                    int32 RemainingAmmoCount = 0;

                    // 남은 탄창 개수 계산
                    for (const FItemData& Item : AmmoItems)
                    {
                        RemainingAmmoCount += Item.ItemCount;
                    }

                    // 남은 탄창 개수를 출력
                    UE_LOG(LogTemp, Log, TEXT("샷건 탄창 %s, 남은 개수: %d개"), *ItemData.ItemName.ToString(), RemainingAmmoCount);
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("현재 장착된 무기가 샷건이 아닙니다."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("장착된 무기가 없습니다."));
    }
}
