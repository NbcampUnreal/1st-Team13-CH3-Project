#include "BAssassinEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

ABAssassinEnemy::ABAssassinEnemy()
{
	MaxHP = 50.f;
	CurrentHP = MaxHP;
	Power = 50.f;
	Speed = 500.f;
	AttackSpeed = 0.7f;
	CoolTime = 5.f;
	SkillDuration = 3.f;
	AttackRange = 150.f;
	bIsRanged = false;

	bIsStealthed = false;
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