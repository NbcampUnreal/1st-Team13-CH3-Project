#include "BRangerEnemy.h"

ABRangerEnemy::ABRangerEnemy()
{
	MaxHP = 150.f;
	CurrentHP = MaxHP;
	Power = 10.f;
	Speed = 300.f;
	AttackSpeed = 2.f;
	CoolTime = 5.f;
	SkillDuration = 5.f;
	AttackRange = 1000.f;
	bIsRanged = true;
	bIsFastAttack = false;
	AttackSpeedIncreaseValue = 2.f;
}

float ABRangerEnemy::GetAttackRange() const
{
	return AttackRange;
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