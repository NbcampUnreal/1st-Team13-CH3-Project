#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BEnemyBase.generated.h"

class ABProjectileBase;

UCLASS()
class SHOOTERGAMEPROJECT_API ABEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	ABEnemyBase();

protected:
	virtual void BeginPlay() override;

	// OnMeleeAttackMontageEnded �Լ� ���� �߰�
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void OnMeleeAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	// Ÿ�̸� �ڵ� (AIController���� ������ �� �ֵ��� public���� ����)
	FTimerHandle AttackTimerHandle;
	FTimerHandle SkillTimerHandle;

public:
	// �ִϸ��̼� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MeleeAttackMontage;

	// ���� ����Ʈ
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI|Components")
	TArray<AActor*> PatrolPoints;

	// �⺻ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	float CurrentHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	float Power;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	float AttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	float CoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	float SkillDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Properties")
	bool bIsRanged;
	// bIsInBattle�� ���� AIController���� �����ϹǷ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Battle")
	bool bIsMeleeAttacking;

	// ����ü ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<ABProjectileBase> ProjectileClass;
	// Accuracy: 1.0�̸� �Ϻ�, 0.0�̸� ���� ����Ȯ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Accuracy;

	// �Լ���
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SpawnProjectile();

	virtual float GetAttackRange() const;
	virtual float GetHP() const;

	UFUNCTION(BlueprintCallable, Category = "Battle")
	virtual void Attack();
	UFUNCTION(BlueprintCallable, Category = "Battle")
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable, Category = "Battle")
	virtual void UseSkill();
	UFUNCTION(BlueprintCallable, Category = "Battle")
	virtual void OnDeath();
	UFUNCTION(BlueprintCallable, Category = "Battle")
	virtual void DropItem();
	UFUNCTION(BlueprintCallable, Category = "Battle")
	virtual void GainHP(float HP);
	void PlayMeleeAttackMontage();
	bool IsMeleeAttacking() const { return bIsMeleeAttacking; }
};