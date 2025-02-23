#include "BEnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "BEnemyAIController.h"
#include "AIController.h"
#include "GameFramework/Controller.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "BCharacter.h"

class ABCharacter;

ABEnemyBase::ABEnemyBase()
{
	MaxHP = 0.f;
	CurrentHP = MaxHP;
	Power = 0.f;
	Speed = 0.f;
	AttackSpeed = 10.f;	// 낮을수록 빠름
	CoolTime = 10.f;	// 낮을수록 빠름
	SkillDuration = 0.f;
	AttackRange = 0.f;
	bIsRanged = false;	//false = 근거리
	bIsInBattle = false;


	//Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	//SetRootComponent(Scene);

	//Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	//Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	//Collision->SetupAttachment(Scene);

	//StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	//StaticMesh->SetupAttachment(Collision);

	AIControllerClass = ABEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	if (SightConfig)
	{
		SightConfig->SightRadius = 1500.f;
		SightConfig->LoseSightRadius = 2000.f;
		SightConfig->PeripheralVisionAngleDegrees = 45.f;
		SightConfig->SetMaxAge(0.f);
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

		AIPerceptionComponent->ConfigureSense(*SightConfig);
		AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	}
	
	
}

void ABEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	if (bIsInBattle)
	{
		GetWorld()->GetTimerManager().SetTimer(SkillTimerHandle, this, &ABEnemyBase::UseSkill, CoolTime + SkillDuration, true);
	}
	
}

float ABEnemyBase::GetAttackRange() const
{ 
	return AttackRange;
}

float ABEnemyBase::GetHP() const
{
	return CurrentHP;
}

void ABEnemyBase::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	ABCharacter* DetectedPlayer = nullptr;
	for (AActor* Actor : UpdatedActors)
	{
		ABCharacter* TempPlayer = Cast<ABCharacter>(Actor);
		if (TempPlayer && GetController() && GetController()->LineOfSightTo(TempPlayer))
		{
			DetectedPlayer = TempPlayer;
			break;
		}
	}

	if (DetectedPlayer)
	{
		// 플레이어와의 거리
		float Distance = FVector::Dist(DetectedPlayer->GetActorLocation(), GetActorLocation());

		//생성자에서(혹은 블루프린트에서) 설정한 최대 시야
		UAISenseConfig_Sight* SightConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>();
		float LoseSightRadius = SightConfig->LoseSightRadius;

		if (Distance <= LoseSightRadius)
		{
			// 플레이어가 가까이 있으면 전투 상태로 전환하고 시야를 180도로 확장
			bIsInBattle = true;
			SetPeripheralVisionAngle(180.f);
		}
		else
		{
			// 플레이어가 멀어지면 전투 상태 해제 및 시야를 45도로 복원
			bIsInBattle = false;
			SetPeripheralVisionAngle(45.f);
		}

		// 주변 1500 내의 다른 AI들에게도 전투 상태를 부여하고 소집 명령(Rally)을 내림
		TArray<AActor*> OtherEnemies;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABEnemyBase::StaticClass(), OtherEnemies);
		for (AActor* Actor : OtherEnemies)
		{
			if (Actor != this && FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 1500.f)
			{
				if (ABEnemyBase* OtherEnemy = Cast<ABEnemyBase>(Actor))
				{
					OtherEnemy->bIsInBattle = true;
					OtherEnemy->Rally();
				}
			}
		}
	}
	else
	{
		// 플레이어가 감지되지 않으면 전투 상태 해제 및 시야를 45도로 복원
		bIsInBattle = false;
		SetPeripheralVisionAngle(45.f);
	}
}

void ABEnemyBase::SetPeripheralVisionAngle(float NewAngle)
{
	if (!AIPerceptionComponent)
		return;

	UAISenseConfig_Sight* SightConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>();
	if (SightConfig)
	{
		SightConfig->PeripheralVisionAngleDegrees = NewAngle;
		AIPerceptionComponent->ConfigureSense(*SightConfig);
	}
}

// Rally 명령 예시: 실제 게임 로직에 맞게 확장 가능
void ABEnemyBase::Rally()
{
	ABCharacter* Player = Cast<ABCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!Player)
	{
		return;
	}

	// 플레이어와의 현재 거리 계산
	float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());

	// 공격 범위(AttackRange) 내에 들어올 때까지 이동 명령 실행
	if (Distance > AttackRange)
	{
		if (AAIController* AICont = Cast<AAIController>(GetController()))
		{
			// Acceptance radius를 AttackRange로 설정하여, 그 거리까지 이동
			AICont->MoveToActor(Player, AttackRange, true, true, true, 0, true);
		}
	}
	else
	{
		// 이미 공격 범위 내에 있다면 공격 실행 또는 다른 전투 로직을 실행할 수 있음
		UE_LOG(LogTemp, Warning, TEXT("%s is within attack range of %s"), *GetName(), *Player->GetName());
	}
}

void ABEnemyBase::Attack()
{
	PlayAttackAnim();
	DealDamageToPlayer();
}

void ABEnemyBase::PlayAttackAnim()
{
	if (AttackMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(AttackMontage);
		}
	}
}

void ABEnemyBase::DealDamageToPlayer()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (PlayerPawn)
	{
		// 플레이어에게 데미지를 적용합니다.
		UGameplayStatics::ApplyDamage(
			PlayerPawn,           // 데미지를 입힐 대상
			Power,				  // 데미지 값
			GetController(),      // 공격을 실행한 컨트롤러
			this,                 // 데미지의 원인(자신)
			UDamageType::StaticClass()  // 데미지 타입 (기본 타입 사용)
		);
	}
}

void ABEnemyBase::UseSkill()
{
	return;
}

float ABEnemyBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0.f, MaxHP);
	
	if (CurrentHP <= 0.f)
	{
		OnDeath();
	}

	return ActualDamage;
}

void ABEnemyBase::OnDeath()
{
	DropItem();
	Destroy();
}

void ABEnemyBase::DropItem()
{

}

void ABEnemyBase::GainHP(float HP)
{
	CurrentHP = FMath::Clamp(CurrentHP + HP, 0.f, MaxHP);
}