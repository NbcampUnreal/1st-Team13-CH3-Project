#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BEnemyBase.generated.h"

class ABProjectileBase;
class ABCharacter;
class ABBaseItem;

UCLASS()
class SHOOTERGAMEPROJECT_API ABEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	ABEnemyBase();

protected:
	virtual void BeginPlay() override;

public:
	// Ÿ�̸� �ڵ� (AIController���� ������ �� �ֵ��� public���� ����)
	FTimerHandle AttackTimerHandle;
	FTimerHandle SkillTimerHandle;

public:

	// ���� ����Ʈ
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI|Components")
	TArray<AActor*> PatrolPoints;

	// �⺻ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	float MaxHP;
	virtual float GetMaxHP() const;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	float CurrentHP;
	virtual float GetCurrentHP() const;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	float Power;
	virtual float GetPower() const;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	float Speed;
	virtual float GetSpeed() const;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	float AttackSpeed;
	virtual float GetAttackSpeed() const;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	float CoolTime;
	virtual float GetCoolTime() const;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	float SkillDuration;
	virtual float GetSkillDuration() const;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	float AttackRange;
	virtual float GetAttackRange() const;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	bool bIsRanged;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	bool bIsDead;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHasDeathBeenCounted;


	//���� ����
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void GrantRewards();

	UPROPERTY(EditDefaultsOnly, Category = "Rewards")
	TSubclassOf<ABBaseItem> WeaponPartItem;
	UPROPERTY(EditDefaultsOnly, Category = "Rewards")
	TSubclassOf<ABBaseItem> HealthKitItem;
	UPROPERTY(EditDefaultsOnly, Category = "Rewards")
	TSubclassOf<ABBaseItem> GrenadeItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Type")
	FString EnemyType;


	UFUNCTION(BlueprintCallable, Category = "Battle")
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable, Category = "Battle")
	virtual void EnemyAttack();
	UFUNCTION(BlueprintCallable, Category = "Battle")
	virtual void UseSkill();
	UFUNCTION(BlueprintCallable, Category = "Battle")
	virtual void OnDeath();
	UFUNCTION(BlueprintCallable, Category = "Battle")
	virtual void DropItem();
	UFUNCTION(BlueprintCallable, Category = "Battle")
	virtual void GainHP(float HP);
	UFUNCTION()
	virtual void DelayedDropAndDestroy();
	UFUNCTION(BlueprintCallable, Category = "Enemy Type")
	virtual FName GetMonsterType() const;
};