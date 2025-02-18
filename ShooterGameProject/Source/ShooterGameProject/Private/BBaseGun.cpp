#include "BBaseGun.h"
#include "BCharacter.h" // 캐릭터 헤더 포함

ABBaseGun::ABBaseGun()
{
    AmmoCount = 30;
    MaxAmmo = 30;
}

void ABBaseGun::Reload(ABCharacter* Character)
{
    if (!Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("캐릭터가 없음!"));
        return;
    }
    // 인벤토리 기능이 있을 시 사용하기 위한 리로드 코드
    //// 캐릭터 인벤토리에서 탄약 개수 확인
    //int32 InventoryAmmo = Character->GetAmmoCount();

    //if (InventoryAmmo > 0)
    //{
    //    int32 NeededAmmo = MaxAmmo - AmmoCount; // 필요한 탄약 개수
    //    int32 AmmoToReload = FMath::Min(NeededAmmo, InventoryAmmo); // 인벤토리에서 충전할 수 있는 만큼 충전

    //    AmmoCount += AmmoToReload;
    //    Character->UseAmmo(AmmoToReload); // 인벤토리에서 탄약 감소

    //    UE_LOG(LogTemp, Log, TEXT("재장전 완료! 탄약: %d/%d"), AmmoCount, MaxAmmo);
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("탄약 부족! 인벤토리에 총알이 없습니다."));
    //}
}

void ABBaseGun::Attack()
{
    if (AmmoCount > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("총 발사! 남은 탄약: %d"), AmmoCount);
        AmmoCount--;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("탄약 부족! 재장전 필요"));
    }
}
