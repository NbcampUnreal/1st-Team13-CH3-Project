// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BItemInterface.h"  // 만들어둔 인터페이스 헤더 포함
#include "ItemStruct.h"
#include "BBaseItem.generated.h"

class USphereComponent;
UCLASS()
class SHOOTERGAMEPROJECT_API ABBaseItem : public AActor, public IBItemInterface
{
	GENERATED_BODY()	
public:	
	// Sets default values for this actor's properties
	ABBaseItem();
	// 아이템을 제거하는 공통 함수 (추가 이펙트나 로직을 넣을 수 있음)
	virtual void DestroyItem();
	FItemData GetItemData() const;
	virtual FName GetItemType() const override;
	void SetItemData(const FItemData& Data);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemType;
	// 아이템 이름을 편집 가능하게 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemName;
	// 아이템 이름을 편집 가능하게 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemInfo")
	FItemData ItemData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USceneComponent* Scene;
	// 충돌 컴포넌트 (플레이어 진입 범위 감지용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* Collision;
	// 아이템 시각 표현용 스태틱 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	UStaticMeshComponent* StaticMesh;
	bool bIsDragging = false;
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

public:
	virtual FName GetItemName() const override;
	virtual void UseItem(AActor* Activator) override;
};
