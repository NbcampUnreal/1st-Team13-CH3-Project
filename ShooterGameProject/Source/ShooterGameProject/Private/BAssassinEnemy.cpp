#include "BAssassinEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

ABAssassinEnemy::ABAssassinEnemy()
{
	MaxHP = 250.f;
	CurrentHP = MaxHP;
	Power = 50.f;
	Speed = 550.f;
	AttackSpeed = 0.7f;
	CoolTime = 5.f;
	SkillDuration = 3.f;
	AttackRange = 150.f;
	bIsRanged = false;
	EnemyType = "Assassin";

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

FName ABAssassinEnemy::GetMonsterType() const
{
	return FName(TEXT("Assassin"));
}