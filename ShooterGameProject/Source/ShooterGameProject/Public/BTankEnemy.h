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

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FTimerHandle ExplosionTimerHandle;

	void BeginPlay() override;
	void UseSkill() override;
	UFUNCTION(BlueprintCallable)
	void Explode();
	void EndSkill();
	virtual FName GetMonsterType() const override;
};
