#include "BEnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "BEnemyAIController.h"
#include "AIController.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "BCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BProjectileBase.h"
#include "BGameInstance.h"
#include "BUIManager.h"
#include "DrawDebugHelpers.h"
#include "BPlayerState.h"

class ABCharacter;

ABEnemyBase::ABEnemyBase()
{
	MaxHP = 0.f;
	CurrentHP = MaxHP;
	Power = 0.f;
	Speed = 0.f;
	AttackSpeed = 10.f; // 낮을수록 빠름
	CoolTime = 10.f;    // 낮을수록 빠름
	SkillDuration = 0.f;
	AttackRange = 0.f;
	bIsRanged = false;   // false = 근거리
	bIsDead = false;

	// AIControllerClass 지정 → AIController가 감지 로직을 담당
	AIControllerClass = ABEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

float ABEnemyBase::GetMaxHP() const
{
	return MaxHP;
}

float ABEnemyBase::GetCurrentHP() const
{
	return CurrentHP;
}

float ABEnemyBase::GetPower() const
{
	return Power;
}

float ABEnemyBase::GetSpeed() const
{
	return Speed;
}

float ABEnemyBase::GetAttackSpeed() const
{
	return AttackSpeed;
}

float ABEnemyBase::GetCoolTime() const
{
	return CoolTime;
}

float ABEnemyBase::GetSkillDuration() const
{
	return SkillDuration;
}

float ABEnemyBase::GetAttackRange() const
{
	return AttackRange;
}

FName ABEnemyBase::GetMonsterType() const
{
	return FName(TEXT("Basic"));
}

void ABEnemyBase::UseSkill()
{
	return;
}

float ABEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0.f, MaxHP);
	if (CurrentHP <= 0.f)
	{
		bIsDead = true;
	}
	return ActualDamage;
}

void ABEnemyBase::OnDeath()
{
	if (UBGameInstance* GameInstance = GetGameInstance<UBGameInstance>())
	{
		if (UBUIManager* UIManagerInstance = Cast<UBUIManager>(GameInstance->GetUIManagerInstance()))
		{
			UIManagerInstance->UpdateKillLog(GetMonsterType());
		}
	}

	if (GetMesh())
	{
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	if (GetWorld())
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABEnemyBase::DelayedDropAndDestroy, 3.f, false);
	}

}

void ABEnemyBase::DelayedDropAndDestroy()
{
	GrantRewards();
	DropItem();
	Destroy();
}

void ABEnemyBase::GainHP(float HP)
{
	CurrentHP = FMath::Clamp(CurrentHP + HP, 0.f, MaxHP);
}

void ABEnemyBase::GrantRewards()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;
	ABPlayerState* BPlayerState = PlayerController->GetPlayerState<ABPlayerState>();
	if (!BPlayerState) return;

	int32 GoldReward = 0;
	int32 ExpReward = 0;

	if (EnemyType == "Basic")
	{
		GoldReward = 10;
		ExpReward = 100;
	}
	else
	{
		GoldReward = 50;
		ExpReward = 200;
	}

	BPlayerState->AddCoin(GoldReward);
	BPlayerState->AddExp(ExpReward);

	UE_LOG(LogTemp, Log, TEXT("%s destroyed! %d Gold, %d EXP Gained!"), *EnemyType, GoldReward, ExpReward);
}

void ABEnemyBase::DropItem()
{
	UWorld* World = GetWorld();
	if (!World) return;

	float RandomValue = FMath::FRandRange(0.0f, 100.0f);
	FVector DropLocation = GetActorLocation() + FVector(FMath::RandRange(-50.f, 50.f), FMath::RandRange(-50.f, 50.f), 0.f);

	if (EnemyType == "Ranger" && RandomValue < 10.0f) // 10% 확률로 무기 파츠 드랍
	{
		if (WeaponPartItem)
		{
			World->SpawnActor<ABBaseItem>(WeaponPartItem, DropLocation, FRotator::ZeroRotator);
			UE_LOG(LogTemp, Log, TEXT("무기 파츠 드랍됨!"));
		}
	}

	else if (EnemyType == "Mage" && RandomValue < 50.0f) // 20% 확률로 회복약 드랍 //test50%
	{
		if (HealthKitItem)
		{
			World->SpawnActor<ABBaseItem>(HealthKitItem, DropLocation, FRotator::ZeroRotator);
			UE_LOG(LogTemp, Log, TEXT("회복약 드랍됨!"));
		}
	}

	else if (EnemyType == "Tank") // 탱커 몹 → 수류탄 드랍 (미구현 시 아무것도 드랍 안 함)
	{
		if (GrenadeItem)
		{
			World->SpawnActor<ABBaseItem>(GrenadeItem, DropLocation, FRotator::ZeroRotator);
			UE_LOG(LogTemp, Log, TEXT("수류탄 드랍됨!"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("탱커 처치했으나 드랍 아이템 없음"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("아이템 없음"));
	}
}
