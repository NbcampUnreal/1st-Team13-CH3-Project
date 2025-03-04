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
	Accuracy = 0.7f;
	bIsRanged = false;   // false = 근거리
	bIsMeleeAttacking = false;
	MeleeAttackMontage = nullptr;
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

FName ABEnemyBase::GetEnemyName() const
{
	return FName(TEXT("Base"));
}

void ABEnemyBase::AttackPlayer()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (PlayerPawn)
	{
		if (bIsRanged)
		{
			SpawnProjectile();
		}
		else
		{
			UGameplayStatics::ApplyDamage(
				PlayerPawn,
				Power,
				GetController(),
				this,
				UDamageType::StaticClass()
			);
		}
	}
}

void ABEnemyBase::UseSkill()
{
	return;
}

//float ABEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
//	CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0.f, MaxHP);
//	if (CurrentHP <= 0.f)
//	{
//		OnDeath();
//	}
//	return ActualDamage;
//}

void ABEnemyBase::EnemyOnDeath()
{
	DropItem();
	Destroy();

	if (UBGameInstance* GameInstance = GetGameInstance<UBGameInstance>())
	{
		if (UBUIManager* UIManagerInstance = Cast<UBUIManager>(GameInstance->GetUIManagerInstance()))
		{
			UIManagerInstance->UpdateKillLog(GetEnemyName());
		}
	}
}

void ABEnemyBase::DropItem()
{
	// 아이템 드랍 로직 구현 (필요 시)
}

void ABEnemyBase::GainHP(float HP)
{
	CurrentHP = FMath::Clamp(CurrentHP + HP, 0.f, MaxHP);
}

void ABEnemyBase::PlayMeleeAttackMontage()
{
	if (!MeleeAttackMontage)
		return;

	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(MeleeAttackMontage);
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &ABEnemyBase::OnMeleeAttackMontageEnded);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, MeleeAttackMontage);
		bIsMeleeAttacking = true;
	}
}

void ABEnemyBase::OnMeleeAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == MeleeAttackMontage)
	{
		bIsMeleeAttacking = false;
	}
}

void ABEnemyBase::SpawnProjectile()
{
	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileClass is not set!"));
		return;
	}

	FVector SpawnLocation = GetActorLocation();
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerPawn not found!"));
		return;
	}

	FVector TargetLocation = PlayerPawn->GetActorLocation();
	FVector ShootDirection = (TargetLocation - SpawnLocation).GetSafeNormal();
	FRotator MuzzleRotation = ShootDirection.Rotation();

	float MaxSpreadAngle = 10.0f;
	float CurrentSpread = MaxSpreadAngle * (1.0f - Accuracy);
	float RandomPitch = FMath::FRandRange(-CurrentSpread, CurrentSpread);
	float RandomYaw = FMath::FRandRange(-CurrentSpread, CurrentSpread);
	FRotator FinalRotation = MuzzleRotation;
	FinalRotation.Pitch += RandomPitch;
	FinalRotation.Yaw += RandomYaw;

	FVector FinalDirection = FinalRotation.Vector();
	UWorld* World = GetWorld();
	if (World)
	{
		ABProjectileBase* Projectile = World->SpawnActor<ABProjectileBase>(ProjectileClass, SpawnLocation, FinalRotation);
		if (Projectile)
		{
			Projectile->FireInDirection(FinalDirection);
		}
	}
}
