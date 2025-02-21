#include "BAssassinEnemy.h"

ABAssassinEnemy::ABAssassinEnemy()
{
	MaxHP = 50.f;
	CurrentHP = MaxHP;
	Power = 30.f;
	Speed = 400.f;
	AttackSpeed = 0.7f;
	CoolTime = 5.f;
	SkillDuration = 3.f;
	AttackRange = 100.f;
	bIsRanged = false;
	bIsStealthed = false;
}

float ABAssassinEnemy::GetAttackRange() const
{
	return AttackRange;
}

void ABAssassinEnemy::UseSkill()
{
	bIsStealthed = true;
	if (GetMesh())
	{
		GetMesh()->SetVisibility(false, true);
	}
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABAssassinEnemy::EndSkill, SkillDuration, true);
}

void ABAssassinEnemy::EndSkill()
{
	bIsStealthed = false;
	if (GetMesh())
	{
		GetMesh()->SetVisibility(true, true);
	}
}