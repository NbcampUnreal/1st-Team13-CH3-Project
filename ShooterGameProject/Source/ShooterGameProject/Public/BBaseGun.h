#pragma once

#include "CoreMinimal.h"
#include "BBaseWeapon.h"
#include "BBaseGun.generated.h"

class ABCharacter; // 캐릭터 클래스 전방 선언

UCLASS()
class SHOOTERGAMEPROJECT_API ABBaseGun : public ABBaseWeapon
{
	GENERATED_BODY()

public:
	ABBaseGun();

	// 탄약 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	int32 AmmoCount;

	// 최대 탄약 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	int32 MaxAmmo;

	// 재장전 기능
	UFUNCTION(BlueprintCallable, Category = "Gun")
	void Reload(ABCharacter* Character);

	// 총기 공격 동작 (발사)
	virtual void Attack() override;
};
