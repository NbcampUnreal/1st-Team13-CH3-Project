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
	// bIsInBattle는 이제 AIController에서 관리하므로 제거
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