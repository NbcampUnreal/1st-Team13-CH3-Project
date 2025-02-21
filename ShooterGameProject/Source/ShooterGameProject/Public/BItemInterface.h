// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SHOOTERGAMEPROJECT_API IBItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0;
	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0;
	// 장비, 돈, 총알
	// 상점 구매용 아이템 포션 등의 고려
	virtual void ActivateItem(AActor* Activator) = 0;
	virtual FName GetItemType() const = 0;
	virtual FName GetItemName() const = 0;
	
};
