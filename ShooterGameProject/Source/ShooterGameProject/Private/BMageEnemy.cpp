#include "BMageEnemy.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BEnemyBase.h"

ABMageEnemy::ABMageEnemy()
{
	MaxHP = 100.f;
	CurrentHP = MaxHP;
	Power = 10.f;
	Speed = 200.f;
	AttackSpeed = 3.f;
	CoolTime = 10.f;
	AttackRange = 600.f;
	bIsRanged = true;
	HealingRadius = 1500.f;
	HealAmount = 20.f;

	HealingCollision = CreateDefaultSubobject<USphereComponent>(TEXT("HealingCollision"));
	HealingCollision->InitSphereRadius(HealingRadius);
	HealingCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	HealingCollision->SetupAttachment(Scene);
}

float ABMageEnemy::GetAttackRange() const
{
	return AttackRange;
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