#include "BBasicEnemy.h"

ABBasicEnemy::ABBasicEnemy()//스킬 없음
{
	MaxHP = 200.f;
	CurrentHP = MaxHP;
	Power = 10.f;
	Speed = 400.f;
	AttackSpeed = 2.f;
	CoolTime = 0.f;
	AttackRange = 150.f;
	bIsRanged = false;
}

FName ABBasicEnemy::GetEnemyName() const
{
	return FName(TEXT("Basic Enemy"));
}