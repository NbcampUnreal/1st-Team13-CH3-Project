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
	AttackSpeed = 10.f;// �������� ����
	CoolTime = 10.f;// �������� ����
	SkillDuration = 0.f;
	AttackRange = 0.f;
	Accuracy = 0.7f;
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
	//updatedactors -> bcharacter�� cast
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

	//casting�� actor���� �Ÿ��� ���� ������� ����
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


	//���� ���¶�� Attack(), UseSkill() �Լ��� ���� �ֱ⸶�� ȣ��
	if (bIsInBattle)
	{
		// ���� Ÿ�̸Ӱ� Ȱ��ȭ
		if (!GetWorld()->GetTimerManager().IsTimerActive(AttackTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(
				AttackTimerHandle,       // Ÿ�̸� �ڵ�
				this,                    // Ÿ�̸Ӹ� ȣ���� ������Ʈ
				&ABEnemyBase::Attack,    // ȣ���� �Լ� (Attack �Լ�)
				AttackSpeed,             // �ݺ� ȣ�� ���� (��)
				true                     // �ݺ� ȣ�� ����(true: �ֱ������� ȣ��)
			);
		}
		// ��ų Ÿ�̸Ӱ� Ȱ��ȭ
		if (!GetWorld()->GetTimerManager().IsTimerActive(SkillTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(
				SkillTimerHandle,       // Ÿ�̸� �ڵ�
				this,                    // Ÿ�̸Ӹ� ȣ���� ������Ʈ
				&ABEnemyBase::UseSkill,    // ȣ���� �Լ� (Attack �Լ�)
				CoolTime + SkillDuration,   // �ݺ� ȣ�� ���� (��)
				true                     // �ݺ� ȣ�� ����(true: �ֱ������� ȣ��)
			);
		}
	}
	else
	{
		// ���� ���°� �ƴ϶�� Ÿ�̸Ӱ� Ȱ��ȭ�Ǿ� ���� ��� ����
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
		if (bIsRanged)
		{
			// ����ü�� �����Ͽ�, ����ü�� OnHit �̺�Ʈ���� ������ ó���� �����մϴ�.
			SpawnProjectile();
		}
		else
		{
			// �ٰŸ� ���� AI�� Notify���� ȣ��ǹǷ� ���⼭ �������� ������ ���� �ֽ��ϴ�.
			// (Ȥ�� Notify���� ���� Attack()�� ȣ���ϵ��� ó��)
			UGameplayStatics::ApplyDamage(
				PlayerPawn,           // �������� ���� ���
				Power,				  // ������ ��
				GetController(),      // ������ ������ ��Ʈ�ѷ�
				this,                 // �������� ����(�ڽ�)
				UDamageType::StaticClass()  // ������ Ÿ�� (�⺻ Ÿ�� ���)
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

void ABEnemyBase::SpawnProjectile()
{
	// ProjectileClass�� �����Ǿ� �ִ��� Ȯ��
	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileClass is not set!"));
		return;
	}

	// �߻� ��ġ: ���⼭�� ĳ������ ��ġ�� ���������, ���� ��ġ(��: "Muzzle")�� ����� ���� �ֽ��ϴ�.
	FVector SpawnLocation = GetActorLocation();

	// �÷��̾�(�Ǵ� ��ǥ)�� ������. ���⼭�� ù ��° �÷��̾� Pawn�� ����մϴ�.
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerPawn not found!"));
		return;
	}

	// ��ǥ ���� ���: �÷��̾� ��ġ���� SpawnLocation�� ���� ���� ���ͷ� ����ϴ�.
	FVector TargetLocation = PlayerPawn->GetActorLocation();
	FVector ShootDirection = (TargetLocation - SpawnLocation).GetSafeNormal();

	// �⺻ �߻� ȸ��: ��ǥ ������ ȸ�� ������ ��ȯ�մϴ�.
	FRotator MuzzleRotation = ShootDirection.Rotation();

	// ���߷��� ���� ��������(����)�� �����մϴ�.
	// ��: MaxSpreadAngle�� 10���� �� Accuracy�� 1�̸� ���� 0, Accuracy�� 0�̸� �ִ� 10�� ����.
	float MaxSpreadAngle = 10.0f; // �ִ� �������� ���� (���� ����)
	float CurrentSpread = MaxSpreadAngle * (1.0f - Accuracy); // Accuracy�� �������� ������ Ŀ��.

	// ���� ����: Pitch�� Yaw�� ���� -CurrentSpread ~ +CurrentSpread ������ ������ �߰�
	float RandomPitch = FMath::FRandRange(-CurrentSpread, CurrentSpread);
	float RandomYaw = FMath::FRandRange(-CurrentSpread, CurrentSpread);
	FRotator FinalRotation = MuzzleRotation;
	FinalRotation.Pitch += RandomPitch;
	FinalRotation.Yaw += RandomYaw;

	// ���� �߻� ������ ���
	FVector FinalDirection = FinalRotation.Vector();

	// UWorld���� ����ü�� ����
	UWorld* World = GetWorld();
	if (World)
	{
		ABProjectileBase* Projectile = World->SpawnActor<ABProjectileBase>(ProjectileClass, SpawnLocation, FinalRotation);
		if (Projectile)
		{
			// ����ü�� �߻� ����� �ӵ��� ����
			Projectile->FireInDirection(FinalDirection);
		}
	}
}