#include "BRangerEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

ABRangerEnemy::ABRangerEnemy()
{
	MaxHP = 150.f;
	CurrentHP = MaxHP;
	Power = 20.f;
	Speed = 300.f;
	AttackSpeed = 2.f;
	CoolTime = 5.f;
	SkillDuration = 5.f;
	AttackRange = 2000.f;
	bIsRanged = true;
	EnemyType = "Ranger";

	bIsFastAttack = false;
	AttackSpeedIncreaseValue = 2.f;
}

float ABRangerEnemy::GetAttackSpeedIncreaseValue()
{
	return AttackSpeedIncreaseValue;
}

void ABRangerEnemy::UseSkill()
{
	bIsFastAttack = true;
	AttackSpeed = AttackSpeed * AttackSpeedIncreaseValue;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABRangerEnemy::EndSkill, SkillDuration, false);
}

void ABRangerEnemy::EndSkill()
{
	bIsFastAttack = false;
	AttackSpeed = AttackSpeed / AttackSpeedIncreaseValue;
}

FName ABRangerEnemy::GetMonsterType() const
{
	return FName(TEXT("Ranger"));
}