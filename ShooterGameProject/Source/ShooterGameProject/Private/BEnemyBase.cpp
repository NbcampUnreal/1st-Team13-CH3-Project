#include "BEnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "BEnemyAIController.h"
#include "AIController.h"
#include "GameFramework/Controller.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "BCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

class ABCharacter;

ABEnemyBase::ABEnemyBase()
{
	MaxHP = 0.f;
	CurrentHP = MaxHP;
	Power = 0.f;
	Speed = 0.f;
	AttackSpeed = 10.f;// �������� ����
	CoolTime = 10.f;// �������� ����
	SkillDuration = 0.f;
	AttackRange = 0.f;
	bIsRanged = false;//false = �ٰŸ�
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
	if (bIsInBattle)
	{
		GetWorld()->GetTimerManager().SetTimer(SkillTimerHandle, this, &ABEnemyBase::UseSkill, CoolTime + SkillDuration, true);
	}
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
		// �÷��̾���� �Ÿ�
		float Distance = FVector::Dist(DetectedPlayer->GetActorLocation(), GetActorLocation());

		//�����ڿ���(Ȥ�� �������Ʈ����) ������ �ִ� �þ�
		UAISenseConfig_Sight* SightConfig = AIPerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>();
		float LoseSightRadius = SightConfig->LoseSightRadius;
		AAIController* AICont = Cast<AAIController>(GetController());

		if (Distance <= LoseSightRadius)
		{
			// �÷��̾ ������ ������ ���� ���·� ��ȯ�ϰ� �þ� ����
			bIsInBattle = true;
			if (AICont)
			{
				AICont->SetFocus(DetectedPlayer);
			}
		}
		else
		{
			// �÷��̾ �־����� ���� ���� ���� �� �þ� ���� ����
			bIsInBattle = false;
			if (AICont)
			{
				AICont->ClearFocus(EAIFocusPriority::Gameplay);
			}
		}

		if (ABEnemyAIController* EnemyAIController = Cast<ABEnemyAIController>(GetController()))
		{
			UBlackboardComponent* BlackboardComp = EnemyAIController->GetBlackboardComponent();
			if (BlackboardComp)
			{
				//BlackboardComp->SetValueAsObject("TargetActor", DetectedPlayer);
			}
		}

		// �ֺ� 1500 ���� �ٸ� AI�鿡�Ե� ���� ���¸� �ο��ϰ� ���� ���(Rally)�� ����
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
		// �÷��̾ �������� ������ ���� ���� ���� �� �þ� ���� ����
		bIsInBattle = false;

		if (AAIController* AICont = Cast<AAIController>(GetController()))
		{
			AICont->StopMovement();
		}
	}
}

// Rally ��� ����: ���� ���� ������ �°� Ȯ�� ����
void ABEnemyBase::Rally()
{
	ABCharacter* Player = Cast<ABCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!Player)
	{
		return;
	}

	// �÷��̾���� ���� �Ÿ� ���
	float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());

	// ���� ����(AttackRange) ���� ���� ������ �̵� ��� ����
	if (Distance > AttackRange)
	{
		if (AAIController* AICont = Cast<AAIController>(GetController()))
		{
			// Acceptance radius�� AttackRange�� �����Ͽ�, �� �Ÿ����� �̵�
			AICont->MoveToActor(Player, AttackRange, true, true, true, 0, true);
		}
	}
	else
	{
		// �̹� ���� ���� ���� �ִٸ� ���� ���� �Ǵ� �ٸ� ���� ������ ������ �� ����
		UE_LOG(LogTemp, Warning, TEXT("%s is within attack range of %s"), *GetName(), *Player->GetName());
	}
}

void ABEnemyBase::Attack()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (PlayerPawn)
	{
		// �÷��̾�� �������� �����մϴ�.
		UGameplayStatics::ApplyDamage(
			PlayerPawn,           // �������� ���� ���
			Power,				  // ������ ��
			GetController(),      // ������ ������ ��Ʈ�ѷ�
			this,                 // �������� ����(�ڽ�)
			UDamageType::StaticClass()  // ������ Ÿ�� (�⺻ Ÿ�� ���)
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

void ABEnemyBase::PlayMeleeAttackMontage()
{
	if (!MeleeAttackMontage) return;

	// ���̷�Ż �޽ÿ� ����� AnimInstance�� ������
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (AnimInstance)
	{
		// ��Ÿ�� ���
		AnimInstance->Montage_Play(MeleeAttackMontage);

		// ��Ÿ�ְ� ������ ��(����ƿ� ����) ����� ��������Ʈ ���
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &ABEnemyBase::OnMeleeAttackMontageEnded);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, MeleeAttackMontage);

		// ���� �� ���·� ����
		bIsMeleeAttacking = true;
	}
}

void ABEnemyBase::OnMeleeAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// ���� ���� ���̴� MeleeAttackMontage�� ���� ������ Ȯ��
	if (Montage == MeleeAttackMontage)
	{
		// ���� ���� ���·� ����
		bIsMeleeAttacking = false;
	}
}