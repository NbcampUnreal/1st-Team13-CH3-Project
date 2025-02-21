#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "BEnemyBase.generated.h"

class UCapsuleComponent;

UCLASS()
class SHOOTERGAMEPROJECT_API ABEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	ABEnemyBase();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

public:	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	UCapsuleComponent* Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* AttackMontage;


	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI|Components")
	TArray<AActor*> PatrolPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Properties")
	float MaxHP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Properties")
	float CurrentHP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Properties")
	float Power;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Properties")
	float Speed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Properties")
	float AttackSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Properties")
	float CoolTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	float SkillDuration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Properties")
	float AttackRange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Properties")
	bool bIsRanged;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	bool bIsInBattle;
	


	FTimerHandle SkillTimerHandle;

	virtual float GetAttackRange() const;
	virtual float GetHP() const;

	virtual void Attack();
	UFUNCTION(BlueprintCallable, Category = "Animation")
	virtual void PlayAttackAnim();
	virtual void DealDamageToPlayer();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable, Category = "Skill")
	virtual void UseSkill();
	virtual void OnDeath();
	virtual void DropItem();
	virtual void GainHP(float HP);
	virtual void SetPeripheralVisionAngle(float NewAngle);
	virtual void Rally();
};
