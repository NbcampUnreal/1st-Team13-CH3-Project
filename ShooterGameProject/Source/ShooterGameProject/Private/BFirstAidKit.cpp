// Fill out your copyright notice in the Description page of Project Settings.


#include "BFirstAidKit.h"
#include "BCharacter.h"
#include "BPlayerState.h"
#include "Components/SphereComponent.h"
ABFirstAidKit::ABFirstAidKit()
{
	ItemType = "Consumeable";
	ItemName = "FirstAidKit";
	RecoveryHealth = 50.0f;

	// FItemData 초기화
	FItemData NewItemData;
	NewItemData.ItemName = TEXT("FirstAidKit");
	NewItemData.ItemClass = ABFirstAidKit::StaticClass();
	NewItemData.ItemCount = 1;  // 기본적으로 1개씩 추가
	NewItemData.ItemIcon = nullptr;  // 필요 시 아이콘 설정
	NewItemData.ItemRef = this;

	// SetItemData를 통해 ItemData 설정
	SetItemData(NewItemData);
}

void ABFirstAidKit::ActivateItem(AActor* Activator)
{
	// 오버랩 됐을 때 드래그 & 드롭 되게

}

FName ABFirstAidKit::GetItemType() const
{
	return ItemType;
}

FName ABFirstAidKit::GetItemName() const
{
	return ItemName;
}

void ABFirstAidKit::UseItem(AActor* Activator)
{
    if (!Activator) return;

    ABCharacter* Character = Cast<ABCharacter>(Activator);
    if (!Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ Activator가 ABCharacter가 아닙니다!"));
        return;
    }

    // ✅ 플레이어 상태 가져오기
    ABPlayerState* PlayerState = Character->GetPlayerState<ABPlayerState>();
    if (!PlayerState)
    {
        UE_LOG(LogTemp, Warning, TEXT("❌ PlayerState를 찾을 수 없습니다!"));
        return;
    }

    // ✅ 체력 회복
    PlayerState->AddCurrentHealth(RecoveryHealth);  // 체력 증가 함수 호출
    UE_LOG(LogTemp, Log, TEXT("✅ 구급상자 사용: 체력 +%.1f, 현재 체력: %.1f"),
        RecoveryHealth, PlayerState->GetCurrentHealth());

    // ✅ 인벤토리에서 구급상자 아이템 제거
    PlayerState->InventoryRemoveItem(ItemData); // 인벤토리에서 구급상자 제거

    // ✅ 구급상자 아이템 사용 후 남은 개수 출력
    TArray<FItemData> FirstAidKitItems = PlayerState->GetInventoryTypeItem(ItemData.ItemName);
    int32 RemainingFirstAidKitCount = 0;

    for (const FItemData& Item : FirstAidKitItems)
    {
        RemainingFirstAidKitCount += Item.ItemCount;
    }

    UE_LOG(LogTemp, Log, TEXT("구급상자 %s, 남은 개수: %d개"), *ItemData.ItemName.ToString(), RemainingFirstAidKitCount);
}

