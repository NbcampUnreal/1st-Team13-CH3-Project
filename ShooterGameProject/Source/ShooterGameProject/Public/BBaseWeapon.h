
#pragma once

#include "CoreMinimal.h"
#include "BBaseItem.h"
#include "BBaseWeapon.generated.h"

/**
 * 
 */
class ABCharacter; // 캐릭터 클래스 전방 선언
UCLASS(Blueprintable)  // 블루프린트에서 사용 가능하도록 설정
class SHOOTERGAMEPROJECT_API ABBaseWeapon : public ABBaseItem
{
	GENERATED_BODY()
	
public:
	ABBaseWeapon();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FString WeaponType;


	// 무기의 데미지 (자식 클래스에서 상속받아 값만 바꿔줄 예정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 WeaponDamage;

	// 무기의 이름 (자식 클래스에서 상속받아 값만 바꿔줄 예정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponName;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireRate;
	UPROPERTY()
	ABCharacter* Character;  // 무기를 장착한 캐릭터

	// 부모 클래스에서 상속받은 ActivateItem 함수를 오버라이드
	virtual void ActivateItem(AActor* Activator) override;
	void EquipWeapon(AActor* Character);
	void SetOwnerCharacter(ABCharacter* NewOwner);

	// 공격 동작 (자식 클래스에서 오버라이드)
	virtual void Attack();

	// 물리 시뮬레이션 끄는 처리 (각 자식 클래스에서 처리하도록 유도)
	virtual void DisablePhysicsSimulation();
	// 무기 소유 캐릭터
	UPROPERTY()
	ABCharacter* OwnerCharacter;
	
};
