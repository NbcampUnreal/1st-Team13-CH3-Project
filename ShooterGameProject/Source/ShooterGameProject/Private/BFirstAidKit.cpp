// Fill out your copyright notice in the Description page of Project Settings.


#include "BFirstAidKit.h"
#include "BPlayerState.h"
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
	// 오버랩 됐을 때 인벤토리에 아이템 추가(퀵슬롯으로)
	if (Activator && Activator->ActorHasTag("Player"))
	{
		//FirstAidKitCount를 더해주고 해당 액터 삭제 / 캐릭터에 구급상자 카운트가 0이면 사용안되게 만들기.
		ABCharacter* OverlappingCharacter = Cast<ABCharacter>(Activator);
		OverlappingCharacter->FirstAidKitCount++;
		UE_LOG(LogTemp, Warning, TEXT("FirstAidKitCount: %d"), OverlappingCharacter->FirstAidKitCount);
		Destroy();
	}
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

		// ✅ 구급상자 개수 감소
		OverlappingCharacter->FirstAidKitCount--;

		// ✅ 디버그 로그 출력
		UE_LOG(LogTemp, Log, TEXT("✅ Used FirstAidKit: Health %f, Remaining Kits: %d, Current HP: %f"),
			RecoveryHealth, OverlappingCharacter->FirstAidKitCount, PlayerState -> GetCurrentHealth());
	}
}

