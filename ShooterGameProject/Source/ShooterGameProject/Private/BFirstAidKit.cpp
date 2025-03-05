// Fill out your copyright notice in the Description page of Project Settings.


#include "BFirstAidKit.h"
#include "BCharacter.h"
#include "Components/SphereComponent.h"
ABFirstAidKit::ABFirstAidKit()
{
	ItemType = "Consumeable";
	ItemName = "FirstAidKit";
	RecoveryHealth = 50.0f;
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
	if (Activator && Activator->ActorHasTag("Player"))
	{
		ABCharacter* OverlappingCharacter = Cast<ABCharacter>(Activator);
		if (!OverlappingCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("❌ OverlappingCharacter is nullptr!"));
			return;
		}

		// ✅ 캐릭터의 PlayerState 가져오기
		ABPlayerState* PlayerState = Cast<ABPlayerState>(OverlappingCharacter->GetPlayerState());
		if (!PlayerState)
		{
			UE_LOG(LogTemp, Warning, TEXT("❌ PlayerState is nullptr!"));
			return;
		}

		// ✅ 체력 회복 로직
		PlayerState->AddCurrentHealth(RecoveryHealth); // PlayerState에서 체력 증가 함수 호출

		// ✅ 디버그 로그 출력
		UE_LOG(LogTemp, Log, TEXT("✅ Used FirstAidKit: Health +%.1f,CurrentHealth: %f, Remaining Kits: %d"),
			RecoveryHealth, PlayerState->GetCurrentHealth(), OverlappingCharacter->FirstAidKitCount);
	}
}
