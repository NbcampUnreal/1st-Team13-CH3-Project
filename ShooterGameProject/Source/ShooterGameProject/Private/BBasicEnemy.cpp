#include "BBasicEnemy.h"

ABBasicEnemy::ABBasicEnemy()//��ų ����
{
	MaxHP = 200.f;
	CurrentHP = MaxHP;
	Power = 10.f;
	Speed = 300.f;
	AttackSpeed = 2.f;
	CoolTime = 0.f;
	AttackRange = 70.f;
	bIsRanged = false;
}