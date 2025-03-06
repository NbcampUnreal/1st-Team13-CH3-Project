#include "BTankEnemy.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BGameInstance.h"
#include "BUIManager.h"

ABTankEnemy::ABTankEnemy()
{
	MaxHP = 500.f;
	CurrentHP = MaxHP;
	Power = 30.f;
	Speed = 500.f;
	AttackSpeed = 3.f;
	CoolTime = 5.f;
	SkillDuration = 2.f;
	AttackRange = 150.f;
	bIsRanged = false;
	bIsReflecting = false;
	EnemyType = "Tank";

	ExplosionDamage = 50.f;
	ExplosionDelay = 2.f;
	ExplosionRadius = 300.f;

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(RootComponent);
}

float ABTankEnemy::GetExplosionDamage()
{
	return ExplosionDamage;
}

float ABTankEnemy::GetExplosionDelay()
{
	return ExplosionDelay;
}

float ABTankEnemy::GetExplosionRadius()
{
	return ExplosionRadius;
}

void ABTankEnemy::UseSkill()
{
	bIsReflecting = true;


	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABTankEnemy::EndSkill, SkillDuration, false);
}

void ABTankEnemy::EndSkill()
{
	bIsReflecting = false;
}

void ABTankEnemy::Explode()
{
	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				Actor,
				ExplosionDamage,
				nullptr,
				this,
				UDamageType::StaticClass()
			);
		}
	}
}

FName ABTankEnemy::GetMonsterType() const
{
	return FName(TEXT("Tank"));
}