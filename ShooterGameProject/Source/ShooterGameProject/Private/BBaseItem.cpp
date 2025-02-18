// Fill out your copyright notice in the Description page of Project Settings.


#include "BBaseItem.h"

// Sets default values
ABBaseItem::ABBaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABBaseItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABBaseItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABBaseItem::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ABBaseItem::ActivateItem(AActor* Activator)
{
}

FName ABBaseItem::GetItemType() const
{
	return ItemType;
}

FName ABBaseItem::GetItemName() const
{
	return ItemName;
}

void ABBaseItem::DestroyItem()
{
	// AActor에서 제공하는 Destroy() 함수로 객체 제거
	Destroy();
}



