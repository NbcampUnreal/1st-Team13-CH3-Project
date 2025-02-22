// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BBaseItem.h"
#include "BFirstAidKit.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAMEPROJECT_API ABFirstAidKit : public ABBaseItem
{
	GENERATED_BODY()
	
public:
	ABFirstAidKit();
	//회복량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float RecoveryHealth;

	// BaseItem Override
	 void ActivateItem(AActor* Activator) override;
	 FName GetItemType() const override;
	 FName GetItemName() const override;
	 void UseItem(AActor* Activator) override;
};
