// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BBaseItem.h"
#include "BPistolPart.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAMEPROJECT_API ABPistolPart : public ABBaseItem
{
	GENERATED_BODY()
	ABPistolPart();
public:
	void UseItem(AActor* Activator) override;
	// 파츠의 매쉬 (각 파츠가 하나의 매쉬를 가짐)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RiflePart")
	UStaticMeshComponent* Mesh;
	// 파츠 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RiflePart")
	FString PartName;

	// 파츠의 효과 - 공격력 증대
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RiflePart")
	float DamageBoost;

	// 파츠의 효과 - 발사 속도 감소
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RiflePart")
	float FireRateBoost;

	// 파츠의 효과 - 정확도 증가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RiflePart")
	int32 MaxAmmoBoost;
};
