#include "ABAmmoItem.h"
#include "BBaseGun.h"

ABAmmoItem::ABAmmoItem()
{
}

void ABAmmoItem::ActivateItem(AActor* Activator)
{
    ABBaseGun* Gun = Cast<ABBaseGun>(Activator);
    if (Gun)
    {
        // 🔹 nullptr 대신 탄약을 사용하는 캐릭터(Activator)를 전달해야 함
        Gun->Reload(Activator);

        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("탄약 사용!"));
    }
}
