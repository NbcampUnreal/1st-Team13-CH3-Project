#include "BEnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BEnemyBase.h"
#include "BCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

ABEnemyAIController::ABEnemyAIController()
{
	// AIPerceptionComponent 생성 및 Sight 구성
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

	// OnPerceptionUpdated 이벤트 바인딩
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ABEnemyAIController::OnPerceptionUpdated);
	bIsInBattle = false;



	ABEnemyBase* Enemy = Cast<ABEnemyBase>(GetPawn());
	UBlackboardComponent* BBComp = GetBlackboardComponent();
	if (BBComp)
	{
		BBComp->SetValueAsFloat(TEXT("AttackRange"), Enemy->GetAttackRange());
	}

}

void ABEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ABEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	// 필요한 경우 Blackboard 초기화 등 추가 설정
}

void ABEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	ABEnemyBase* Enemy = Cast<ABEnemyBase>(GetPawn());
	if (!Enemy)
		return;

	// 플레이어 감지
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

	// 감지 결과에 따라 전투 상태 업데이트 및 Blackboard 동기화
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

	// 전투 상태에 따른 Attack 및 Skill 타이머 처리 (기존 코드)
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

	// 전투 상태(bIsInBattle)가 true이면, 3초마다 주변 AI에게 이동 명령을 내리도록 타이머 설정
	if (bIsInBattle)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(RallyTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(
				RallyTimerHandle,
				this,
				&ABEnemyAIController::UpdateRallyMoveCommand,
				3.0f,  // 3초마다 호출
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

	// 현재 AIController의 Blackboard에서 TargetActor(플레이어)를 가져옴
	UBlackboardComponent* BBComp = GetBlackboardComponent();
	ABCharacter* DetectedPlayer = nullptr;
	if (BBComp)
	{
		DetectedPlayer = Cast<ABCharacter>(BBComp->GetValueAsObject(TEXT("TargetActor")));
	}

	// 먼저, DetectedPlayer의 위치가 NavMesh 위에 있는지 확인
	if (DetectedPlayer)
	{
		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (NavSys)
		{
			FNavLocation NavLocation;
			// 50.f 반경의 검색 반경으로 검사 (필요에 따라 조정)
			bool bOnNavMesh = NavSys->ProjectPointToNavigation(DetectedPlayer->GetActorLocation(), NavLocation, FVector(50.f, 50.f, 50.f));
			if (!bOnNavMesh)
			{
				// 플레이어가 NavMesh 밖이라면, 주변 AI의 Blackboard 값을 업데이트하여 공격을 멈춤.
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
				// 디버그 메시지: 플레이어가 NavMesh 밖임을 알림
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Target off NavMesh, clearing aggro"));
				}
				return; // 더 이상 Rally 명령을 내리지 않음
			}
		}
	}

	// 플레이어가 유효하고 NavMesh 위에 있으면, 주변 AI에게 이동 명령과 Blackboard 업데이트를 내림.
	if (DetectedPlayer)
	{
		TArray<AActor*> OtherEnemies;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABEnemyBase::StaticClass(), OtherEnemies);
		for (AActor* Actor : OtherEnemies)
		{
			if (Actor != Enemy)
			{
				float DistBetween = FVector::Dist(Enemy->GetActorLocation(), Actor->GetActorLocation());
				if (DistBetween <= 3000.f) // 예: 3000 단위 이내의 AI에 대해 명령 내림
				{
					ABEnemyBase* OtherEnemy = Cast<ABEnemyBase>(Actor);
					if (OtherEnemy)
					{
						if (AAIController* OtherAICont = Cast<AAIController>(OtherEnemy->GetController()))
						{
							// 각 OtherEnemy의 Blackboard 업데이트
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