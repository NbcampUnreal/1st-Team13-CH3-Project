#pragma once

#include "CoreMinimal.h"
#include "BEnemyBase.h"
#include "BRangerEnemy.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API ABRangerEnemy : public ABEnemyBase
{
	GENERATED_BODY()
	
public:
	ABRangerEnemy();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI|Properties|Ranger")
	bool bIsFastAttack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Properties|Ranger")
	float AttackSpeedIncreaseValue;
	float GetAttackSpeedIncreaseValue();

	void UseSkill() override;
	void EndSkill();
};
