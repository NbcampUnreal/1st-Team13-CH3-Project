// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BItemInterface.h"  // 만들어둔 인터페이스 헤더 포함
#include "BBaseItem.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API ABBaseItem : public AActor, public IBItemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABBaseItem();

protected:
	// 아이템 유형(타입)을 편집 가능하게 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;

	FName ItemName;
	virtual void BeginPlay() override;
	// IItemInterface에서 요구하는 함수들을 반드시 구현
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;
	virtual void ActivateItem(AActor* Activator) override;
	virtual FName GetItemType() const override;
	virtual FName GetItemName() const override;
	// 아이템을 제거하는 공통 함수 (추가 이펙트나 로직을 넣을 수 있음)
	virtual void DestroyItem();

};
