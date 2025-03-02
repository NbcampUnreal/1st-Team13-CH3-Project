#include "BEnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BEnemyBase.h"
#include "BAssassinEnemy.h"
#include "BRangerEnemy.h"
#include "BMageEnemy.h"
#include "BTankEnemy.h"
#include "BCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

ABEnemyAIController::ABEnemyAIController()
{
	// AIPerceptionComponent ���� �� Sight ����
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (SightConfig)
	{
		SightConfig->SightRadius = 1500.f;
		SightConfig->LoseSightRadius = 2000.f;
		SightConfig->PeripheralVisionAngleDegrees = 45.f;
		SightConfig->SetMaxAge(0.f);
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		AIPerceptionComponent->ConfigureSense(*SightConfig);
		AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	}

	// OnPerceptionUpdated �̺�Ʈ ���ε�
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ABEnemyAIController::OnPerceptionUpdated);
	bIsInBattle = false;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/PJH/AI/BT_AI.BT_AI'"));
	if (BT.Succeeded())
	{
		AIBehaviorTree = BT.Object;
	}
	//static ConstructorHelpers::FObjectFinder<UBlackboardData> BB(TEXT("BlackboardData'/Game/PJH/AI/BB_AI.BB_AI'"));
	//if (BB.Succeeded())
	//{
	//	BlackboardData = BB.Object;
	//}
}

void ABEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehaviorTree != nullptr)
	{
		RunBehaviorTree(AIBehaviorTree);
	}
}

void ABEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	
	// �ʿ��� ��� Blackboard �ʱ�ȭ �� �߰� ����
	ABCharacter* TempPlayer = Cast<ABCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	UBlackboardComponent* BBComp = GetBlackboardComponent();
	ABEnemyBase* Enemy = Cast<ABEnemyBase>(GetPawn());
	if (BBComp)
	{
		if (TempPlayer)
		{
			BBComp->SetValueAsObject(TEXT("TargetActor"), TempPlayer);
		}
		BBComp->SetValueAsFloat(TEXT("MaxHP"), Enemy->GetMaxHP());
		BBComp->SetValueAsFloat(TEXT("CurrentHP"), Enemy->GetCurrentHP());
		BBComp->SetValueAsFloat(TEXT("Power"), Enemy->GetPower());
		BBComp->SetValueAsFloat(TEXT("Speed"), Enemy->GetSpeed());
		BBComp->SetValueAsFloat(TEXT("AttackSpeed"), Enemy->GetAttackSpeed());
		BBComp->SetValueAsFloat(TEXT("CoolTime"), Enemy->GetCoolTime());
		BBComp->SetValueAsFloat(TEXT("SkillDuration"), Enemy->GetSkillDuration());
		BBComp->SetValueAsFloat(TEXT("AttackRange"), Enemy->GetAttackRange());

		if (ABAssassinEnemy* Assassin = Cast<ABAssassinEnemy>(GetPawn()))
		{
			BBComp->SetValueAsBool(TEXT("bIsStealthed"), Assassin->bIsStealthed);
		}

		if (ABRangerEnemy* Ranger = Cast<ABRangerEnemy>(GetPawn()))
		{
			BBComp->SetValueAsBool(TEXT("bIsFastAttack"), Ranger->bIsFastAttack);
			BBComp->SetValueAsFloat(TEXT("AttackSpeedIncreaseValue"), Ranger->GetAttackSpeedIncreaseValue());
		}
		if (ABMageEnemy* Mage = Cast<ABMageEnemy>(GetPawn()))
		{
			BBComp->SetValueAsFloat(TEXT("HealingRadius"), Mage->GetHealingRadius());
			BBComp->SetValueAsFloat(TEXT("HealAmount"), Mage->GetHealAmount());
		}
		if (ABTankEnemy* Tank = Cast<ABTankEnemy>(GetPawn()))
		{
			BBComp->SetValueAsFloat(TEXT("ExplosionDamage"), Tank->GetExplosionDamage());
			BBComp->SetValueAsFloat(TEXT("ExplosionDelay"), Tank->GetExplosionDelay());
			BBComp->SetValueAsFloat(TEXT("ExplosionRadius"), Tank->GetExplosionRadius());
			BBComp->SetValueAsBool(TEXT("bIsReflecting"), Tank->bIsReflecting);
		}
	}
}

void ABEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	ABEnemyBase* Enemy = Cast<ABEnemyBase>(GetPawn());
	if (!Enemy)
		return;

	// �÷��̾� ����
	ABCharacter* DetectedPlayer = nullptr;
	for (AActor* Actor : UpdatedActors)
	{
		ABCharacter* TempPlayer = Cast<ABCharacter>(Actor);
		if (TempPlayer && LineOfSightTo(TempPlayer))
		{
			DetectedPlayer = TempPlayer;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Player has been detected"));
			break;
		}
	}

	UBlackboardComponent* BBComp = GetBlackboardComponent();

	// ���� ����� ���� ���� ���� ������Ʈ �� Blackboard ����ȭ
	if (DetectedPlayer)
	{
		bIsInBattle = true;
		SetFocus(DetectedPlayer);

		if (BBComp)
		{
			BBComp->SetValueAsObject(TEXT("TargetActor"), DetectedPlayer);
			BBComp->SetValueAsBool(TEXT("HasSeenPlayer"), true);
		}
	}
	else
	{
		bIsInBattle = false;
		ClearFocus(EAIFocusPriority::Gameplay);
		if (BBComp)
		{
			BBComp->SetValueAsBool(TEXT("HasSeenPlayer"), false);
			BBComp->ClearValue(TEXT("TargetActor"));
		}
	}

	// ���� ���¿� ���� Attack �� Skill Ÿ�̸� ó�� (���� �ڵ�)
	if (Enemy)
	{
		if (bIsInBattle)
		{
			if (!GetWorld()->GetTimerManager().IsTimerActive(Enemy->AttackTimerHandle))
			{
				GetWorld()->GetTimerManager().SetTimer(
					Enemy->AttackTimerHandle,
					Enemy,
					&ABEnemyBase::Attack,
					Enemy->AttackSpeed,
					true
				);
			}
			if (!GetWorld()->GetTimerManager().IsTimerActive(Enemy->SkillTimerHandle))
			{
				GetWorld()->GetTimerManager().SetTimer(
					Enemy->SkillTimerHandle,
					Enemy,
					&ABEnemyBase::UseSkill,
					Enemy->CoolTime + Enemy->SkillDuration,
					true
				);
			}
		}
		else
		{
			if (GetWorld()->GetTimerManager().IsTimerActive(Enemy->AttackTimerHandle))
			{
				GetWorld()->GetTimerManager().ClearTimer(Enemy->AttackTimerHandle);
			}
			if (GetWorld()->GetTimerManager().IsTimerActive(Enemy->SkillTimerHandle))
			{
				GetWorld()->GetTimerManager().ClearTimer(Enemy->SkillTimerHandle);
			}
		}
	}

	// ���� ����(bIsInBattle)�� true�̸�, 3�ʸ��� �ֺ� AI���� �̵� ����� �������� Ÿ�̸� ����
	if (bIsInBattle)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(RallyTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(
				RallyTimerHandle,
				this,
				&ABEnemyAIController::UpdateRallyMoveCommand,
				3.0f,  // 3�ʸ��� ȣ��
				true
			);
		}
	}
	else
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(RallyTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(RallyTimerHandle);
		}
	}
}

void ABEnemyAIController::UpdateRallyMoveCommand()
{
	ABEnemyBase* Enemy = Cast<ABEnemyBase>(GetPawn());
	if (!Enemy)
		return;

	// ���� AIController�� Blackboard���� TargetActor(�÷��̾�)�� ������
	UBlackboardComponent* BBComp = GetBlackboardComponent();
	ABCharacter* DetectedPlayer = nullptr;
	if (BBComp)
	{
		DetectedPlayer = Cast<ABCharacter>(BBComp->GetValueAsObject(TEXT("TargetActor")));
	}

	// ����, DetectedPlayer�� ��ġ�� NavMesh ���� �ִ��� Ȯ��
	if (DetectedPlayer)
	{
		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (NavSys)
		{
			FNavLocation NavLocation;
			// 50.f �ݰ��� �˻� �ݰ����� �˻� (�ʿ信 ���� ����)
			bool bOnNavMesh = NavSys->ProjectPointToNavigation(DetectedPlayer->GetActorLocation(), NavLocation, FVector(50.f, 50.f, 50.f));
			if (!bOnNavMesh)
			{
				// �÷��̾ NavMesh ���̶��, �ֺ� AI�� Blackboard ���� ������Ʈ�Ͽ� ������ ����.
				TArray<AActor*> OtherEnemies;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABEnemyBase::StaticClass(), OtherEnemies);
				for (AActor* Actor : OtherEnemies)
				{
					ABEnemyBase* OtherEnemy = Cast<ABEnemyBase>(Actor);
					if (OtherEnemy)
					{
						if (AAIController* OtherAICont = Cast<AAIController>(OtherEnemy->GetController()))
						{
							UBlackboardComponent* OtherBBComp = OtherAICont->GetBlackboardComponent();
							if (OtherBBComp)
							{
								OtherBBComp->SetValueAsBool(TEXT("HasSeenPlayer"), false);
								OtherBBComp->ClearValue(TEXT("TargetActor"));
							}
						}
					}
				}
				// ����� �޽���: �÷��̾ NavMesh ������ �˸�
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Target off NavMesh, clearing aggro"));
				}
				return; // �� �̻� Rally ����� ������ ����
			}
		}
	}

	// �÷��̾ ��ȿ�ϰ� NavMesh ���� ������, �ֺ� AI���� �̵� ��ɰ� Blackboard ������Ʈ�� ����.
	if (DetectedPlayer)
	{
		TArray<AActor*> OtherEnemies;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABEnemyBase::StaticClass(), OtherEnemies);
		for (AActor* Actor : OtherEnemies)
		{
			if (Actor != Enemy)
			{
				float DistBetween = FVector::Dist(Enemy->GetActorLocation(), Actor->GetActorLocation());
				if (DistBetween <= 3000.f) // ��: 3000 ���� �̳��� AI�� ���� ��� ����
				{
					ABEnemyBase* OtherEnemy = Cast<ABEnemyBase>(Actor);
					if (OtherEnemy)
					{
						if (AAIController* OtherAICont = Cast<AAIController>(OtherEnemy->GetController()))
						{
							// �� OtherEnemy�� Blackboard ������Ʈ
							UBlackboardComponent* OtherBBComp = OtherAICont->GetBlackboardComponent();
							if (OtherBBComp)
							{
								OtherBBComp->SetValueAsBool(TEXT("HasSeenPlayer"), true);
								OtherBBComp->SetValueAsObject(TEXT("TargetActor"), DetectedPlayer);
							}
						}
					}
				}
			}
		}
	}
}

bool ABEnemyAIController::IsPlayerOnNavMesh(ABCharacter* Player) const
{
	if (!Player)
		return false;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSys)
	{
		FNavLocation NavLocation;
		// 50 ���� �ݰ����� �˻�: DetectedPlayer�� ��ġ �ֺ��� NavMesh ��ȿ ������ �ִ��� Ȯ��
		return NavSys->ProjectPointToNavigation(Player->GetActorLocation(), NavLocation, FVector(50.f, 50.f, 200.f));
	}
	return false;
}