#include "BMeleeWeapon.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

ABMeleeWeapon::ABMeleeWeapon()
{
	WeaponDamage = 50.0f;
	WeaponType = "Melee";
	// 1. 씬 컴포넌트를 루트로 설정
	WeaponRoot = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	SetRootComponent(WeaponRoot);
	
	// 2. 핸들(손잡이) 메시 추가
	HandleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandleMesh"));
	HandleMesh->SetupAttachment(WeaponRoot); // 루트에 부착

	// 3. 칼날 메시 추가
	WeaponMesh->SetupAttachment(HandleMesh); // 기본적으로 핸들에 부착
	Collision->SetupAttachment(HandleMesh);
	// 4. 공격 범위 콜리전 추가
	AttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	AttackCollision->SetupAttachment(WeaponMesh); // 칼날에 부착

	// 콜리전 설정
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackCollision->SetCollisionObjectType(ECC_WorldDynamic);
	AttackCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// 충돌 크기 조정
	AttackCollision->SetBoxExtent(FVector(50.0f, 10.0f, 10.0f));
	AttackCollision->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));

	// 충돌 이벤트 바인딩
	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ABMeleeWeapon::OnAttackBeginOverlap);
	AttackCollision->OnComponentEndOverlap.AddDynamic(this, &ABMeleeWeapon::OnAttackEndOverlap);
}

void ABMeleeWeapon::Attack()
{
	Super::Attack();

	// 공격 시작 시 콜리전 활성화
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABMeleeWeapon::OnAttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetOwner()) // 무기 소유자 제외
	{
		UGameplayStatics::ApplyDamage(OtherActor, WeaponDamage, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
		UE_LOG(LogTemp, Warning, TEXT("Melee Attack Hit: %s"), *OtherActor->GetName());
	}
}

void ABMeleeWeapon::OnAttackEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 공격이 끝나면 콜리전 비활성화
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}