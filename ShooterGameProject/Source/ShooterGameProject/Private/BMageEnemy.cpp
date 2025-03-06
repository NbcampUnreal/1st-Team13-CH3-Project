#include "BMageEnemy.h"
#include "Components/SphereComponent.h"
#include "BEnemyBase.h"

ABMageEnemy::ABMageEnemy()
{
	MaxHP = 250.f;
	CurrentHP = MaxHP;
	Power = 10.f;
	Speed = 250.f;
	AttackSpeed = 3.f;
	CoolTime = 10.f;
	AttackRange = 1200.f;
	bIsRanged = true;
	EnemyType = "Mage";

	HealingRadius = 1500.f;
	HealAmount = 20.f;

	HealingCollision = CreateDefaultSubobject<USphereComponent>(TEXT("HealingCollision"));
	HealingCollision->InitSphereRadius(HealingRadius);
	HealingCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	HealingCollision->SetupAttachment(RootComponent);
}

float ABMageEnemy::GetHealingRadius()
{
	return HealingRadius;
}

float ABMageEnemy::GetHealAmount()
{
	return HealAmount;
}

void ABMageEnemy::UseSkill()
{
	TArray<AActor*> OverlappingActors;
	HealingCollision->GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor)
		{
			if (ABEnemyBase* Enemy = Cast<ABEnemyBase>(Actor))
			{
				Enemy->GainHP(HealAmount);
			}
		}
	}
}

FName ABMageEnemy::GetMonsterType() const
{
	return FName(TEXT("Mage"));
}