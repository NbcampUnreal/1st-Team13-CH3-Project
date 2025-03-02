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

	// OnMeleeAttackMontageEnded 함수 선언 추가
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void OnMeleeAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	// 타이머 핸들 (AIController에서 접근할 수 있도록 public으로 선언)
	FTimerHandle AttackTimerHandle;
	FTimerHandle SkillTimerHandle;

public:
	// 애니메이션 관련
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MeleeAttackMontage;

	// 순찰 포인트
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI|Components")
	TArray<AActor*> PatrolPoints;

	// 기본 스탯
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Battle")
	bool bIsMeleeAttacking;

	// 투사체 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<ABProjectileBase> ProjectileClass;
	// Accuracy: 1.0이면 완벽, 0.0이면 가장 부정확
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Accuracy;

	// 함수들
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SpawnProjectile();


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