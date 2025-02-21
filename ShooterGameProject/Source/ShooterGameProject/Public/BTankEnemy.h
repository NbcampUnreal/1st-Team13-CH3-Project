#pragma once

#include "CoreMinimal.h"
#include "BEnemyBase.h"
#include "BTankEnemy.generated.h"

class USphereComponent;

UCLASS()
class SHOOTERGAMEPROJECT_API ABTankEnemy : public ABEnemyBase
{
	GENERATED_BODY()
	
public:
	ABTankEnemy();

	USphereComponent* ExplosionCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties|Tank")
	float ExplosionDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties|Tank")
	float ExplosionDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties|Tank")
	float ExplosionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties|Tank")
	bool bIsReflecting;

	FTimerHandle ExplosionTimerHandle;

	float GetAttackRange() const override;
	void UseSkill() override;
	void OnDeath() override;
	virtual void Explode();
	void EndSkill();
};
