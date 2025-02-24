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
		//체력회복하는 로직 추가예정
		//OverlappingCharacter->Health등으로 하면 될 듯.
	}
}
