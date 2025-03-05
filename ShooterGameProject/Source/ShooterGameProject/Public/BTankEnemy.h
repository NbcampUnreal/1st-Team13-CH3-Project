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
	float GetExplosionDamage();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties|Tank")
	float ExplosionDelay;
	float GetExplosionDelay();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties|Tank")
	float ExplosionRadius;
	float GetExplosionRadius();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties|Tank")
	bool bIsReflecting;

	FTimerHandle ExplosionTimerHandle;

	void UseSkill() override;
	void OnDeath() override;
	virtual void Explode();
	void EndSkill();
};
