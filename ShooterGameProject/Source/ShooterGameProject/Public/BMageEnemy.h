#pragma once

#include "CoreMinimal.h"
#include "BEnemyBase.h"
#include "BMageEnemy.generated.h"

class USphereComponent;

UCLASS()
class SHOOTERGAMEPROJECT_API ABMageEnemy : public ABEnemyBase
{
	GENERATED_BODY()

public:
	ABMageEnemy();
	USphereComponent* HealingCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties|Mage")
	float HealingRadius;
	float GetHealingRadius();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties|Mage")
	float HealAmount;
	float GetHealAmount();

	void BeginPlay() override;
	void UseSkill() override;
	virtual FName GetMonsterType() const override;
};
