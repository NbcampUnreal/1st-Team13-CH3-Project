// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BBaseItem.h"
#include "BBaseWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAMEPROJECT_API ABBaseWeapon : public ABBaseItem
{
	GENERATED_BODY()
	
public:
	ABBaseWeapon();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName WeaponType;


	// 무기의 데미지 (자식 클래스에서 상속받아 값만 바꿔줄 예정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 WeaponDamage;

	// 무기의 이름 (자식 클래스에서 상속받아 값만 바꿔줄 예정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponName;


	// 부모 클래스에서 상속받은 ActivateItem 함수를 오버라이드
	virtual void ActivateItem(AActor* Activator) override;
	void EquipWeapon(AActor* Character);

	// 공격 동작 (자식 클래스에서 오버라이드)
	virtual void Attack();
};
