#pragma once

#include "CoreMinimal.h"
#include "BBaseWeapon.h"
#include "Components/BoxComponent.h"
#include "BMeleeWeapon.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API ABMeleeWeapon : public ABBaseWeapon
{
	GENERATED_BODY()

public:
	ABMeleeWeapon();

	// 공격 함수
	virtual void Attack() override;

protected:

	// 루트 컴포넌트 (전체 무기를 관리하는 씬 컴포넌트)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USceneComponent* WeaponRoot;



	// 핸들 메시 (손잡이)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* HandleMesh;

	// 공격 범위를 위한 충돌 박스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UBoxComponent* AttackCollision;

	// 공격 시작 & 끝 (애니메이션과 연동)
	UFUNCTION()
	void OnAttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnAttackEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
