#include "BEnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "BEnemyAIController.h"
#include "AIController.h"
#include "GameFramework/Controller.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "BCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BProjectileBase.h"

class ABCharacter;

ABEnemyBase::ABEnemyBase()
{
	MaxHP = 0.f;
	CurrentHP = MaxHP;
	Power = 0.f;
	Speed = 0.f;
	AttackSpeed = 10.f;// 낮을수록 빠름
	CoolTime = 10.f;// 낮을수록 빠름
	SkillDuration = 0.f;
	AttackRange = 0.f;
	Accuracy = 0.7f;
	bIsRanged = false;//false = 근거리
	bIsInBattle = false;
	bIsMeleeAttacking = false;
	MeleeAttackMontage = nullptr;

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
	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ABEnemyBase::OnPerceptionUpdated);
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
	//updatedactors -> bcharacter로 cast
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

	//casting된 actor와의 거리를 통해 전투모드 설정
	if (DetectedPlayer)
	{
		// 플레이어와의 거리
		float Distance = FVector::Dist(DetectedPlayer->GetActorLocation(), GetActorLocation());

		//생성자에서(혹은 블루프린트에서) 설정한 최대 시야
		UAISenseConfig_Sight* SightConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>();
		float LoseSightRadius = SightConfig->LoseSightRadius;
		AAIController* AICont = Cast<AAIController>(GetController());

		if (Distance <= LoseSightRadius)
		{
			// 플레이어가 가까이 있으면 전투 상태로 전환하고 시야 고정
			bIsInBattle = true;
			if (AICont)
			{
				AICont->SetFocus(DetectedPlayer);
			}
		}
		else
		{
			// 플레이어가 멀어지면 전투 상태 해제 및 시야 고정 해제
			bIsInBattle = false;
			if (AICont)
			{
				AICont->ClearFocus(EAIFocusPriority::Gameplay);
				AICont->StopMovement();

			}
		}

		if (ABEnemyAIController* EnemyAIController = Cast<ABEnemyAIController>(GetController()))
		{
			UBlackboardComponent* BlackboardComp = EnemyAIController->GetBlackboardComponent();
			if (BlackboardComp)
			{
				BlackboardComp->SetValueAsObject("TargetActor", DetectedPlayer);
			}
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
		// 플레이어가 감지되지 않으면 전투 상태 해제 및 시야 고정 해제
		bIsInBattle = false;

		if (AAIController* AICont = Cast<AAIController>(GetController()))
		{
			AICont->StopMovement();
		}
	}


	//전투 상태라면 Attack(), UseSkill() 함수를 일정 주기마다 호출
	if (bIsInBattle)
	{
		// 공격 타이머가 활성화
		if (!GetWorld()->GetTimerManager().IsTimerActive(AttackTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(
				AttackTimerHandle,       // 타이머 핸들
				this,                    // 타이머를 호출할 오브젝트
				&ABEnemyBase::Attack,    // 호출할 함수 (Attack 함수)
				AttackSpeed,             // 반복 호출 간격 (초)
				true                     // 반복 호출 여부(true: 주기적으로 호출)
			);
		}
		// 스킬 타이머가 활성화
		if (!GetWorld()->GetTimerManager().IsTimerActive(SkillTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(
				SkillTimerHandle,       // 타이머 핸들
				this,                    // 타이머를 호출할 오브젝트
				&ABEnemyBase::UseSkill,    // 호출할 함수 (Attack 함수)
				CoolTime + SkillDuration,   // 반복 호출 간격 (초)
				true                     // 반복 호출 여부(true: 주기적으로 호출)
			);
		}
	}
	else
	{
		// 전투 상태가 아니라면 타이머가 활성화되어 있을 경우 중지
		if (GetWorld()->GetTimerManager().IsTimerActive(AttackTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
		}
		if (GetWorld()->GetTimerManager().IsTimerActive(SkillTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(SkillTimerHandle);
		}
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
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (PlayerPawn)
	{
		if (bIsRanged)
		{
			// 투사체를 스폰하여, 투사체의 OnHit 이벤트에서 데미지 처리를 수행합니다.
			SpawnProjectile();
		}
		else
		{
			// 근거리 공격 AI는 Notify에서 호출되므로 여기서 데미지를 적용할 수도 있습니다.
			// (혹은 Notify에서 직접 Attack()을 호출하도록 처리)
			UGameplayStatics::ApplyDamage(
				PlayerPawn,           // 데미지를 입힐 대상
				Power,				  // 데미지 값
				GetController(),      // 공격을 실행한 컨트롤러
				this,                 // 데미지의 원인(자신)
				UDamageType::StaticClass()  // 데미지 타입 (기본 타입 사용)
			);
		}
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

void ABEnemyBase::PlayMeleeAttackMontage()
{
	if (!MeleeAttackMontage) return;

	// 스켈레탈 메시에 연결된 AnimInstance를 가져옴
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (AnimInstance)
	{
		// 몽타주 재생
		AnimInstance->Montage_Play(MeleeAttackMontage);

		// 몽타주가 끝났을 때(블렌드아웃 포함) 실행될 델리게이트 등록
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &ABEnemyBase::OnMeleeAttackMontageEnded);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, MeleeAttackMontage);

		// 공격 중 상태로 변경
		bIsMeleeAttacking = true;
	}
}

void ABEnemyBase::OnMeleeAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 현재 실행 중이던 MeleeAttackMontage가 끝난 것인지 확인
	if (Montage == MeleeAttackMontage)
	{
		// 공격 종료 상태로 변경
		bIsMeleeAttacking = false;
	}
}

void ABEnemyBase::SpawnProjectile()
{
	// ProjectileClass가 설정되어 있는지 확인
	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileClass is not set!"));
		return;
	}

	// 발사 위치: 여기서는 캐릭터의 위치를 사용하지만, 소켓 위치(예: "Muzzle")를 사용할 수도 있습니다.
	FVector SpawnLocation = GetActorLocation();

	// 플레이어(또는 목표)를 가져옴. 여기서는 첫 번째 플레이어 Pawn을 사용합니다.
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerPawn not found!"));
		return;
	}

	// 목표 방향 계산: 플레이어 위치에서 SpawnLocation을 빼서 단위 벡터로 만듭니다.
	FVector TargetLocation = PlayerPawn->GetActorLocation();
	FVector ShootDirection = (TargetLocation - SpawnLocation).GetSafeNormal();

	// 기본 발사 회전: 목표 방향을 회전 값으로 변환합니다.
	FRotator MuzzleRotation = ShootDirection.Rotation();

	// 명중률에 따른 스프레드(오차)를 적용합니다.
	// 예: MaxSpreadAngle이 10도일 때 Accuracy가 1이면 오차 0, Accuracy가 0이면 최대 10도 오차.
	float MaxSpreadAngle = 10.0f; // 최대 스프레드 각도 (설정 가능)
	float CurrentSpread = MaxSpreadAngle * (1.0f - Accuracy); // Accuracy가 낮을수록 오차가 커짐.

	// 랜덤 오차: Pitch와 Yaw에 대해 -CurrentSpread ~ +CurrentSpread 사이의 랜덤값 추가
	float RandomPitch = FMath::FRandRange(-CurrentSpread, CurrentSpread);
	float RandomYaw = FMath::FRandRange(-CurrentSpread, CurrentSpread);
	FRotator FinalRotation = MuzzleRotation;
	FinalRotation.Pitch += RandomPitch;
	FinalRotation.Yaw += RandomYaw;

	// 최종 발사 방향을 계산
	FVector FinalDirection = FinalRotation.Vector();

	// UWorld에서 투사체를 스폰
	UWorld* World = GetWorld();
	if (World)
	{
		ABProjectileBase* Projectile = World->SpawnActor<ABProjectileBase>(ProjectileClass, SpawnLocation, FinalRotation);
		if (Projectile)
		{
			// 투사체에 발사 방향과 속도를 적용
			Projectile->FireInDirection(FinalDirection);
		}
	}
}