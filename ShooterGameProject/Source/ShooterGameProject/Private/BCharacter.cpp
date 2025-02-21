#include "BCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
ABCharacter::ABCharacter() :
	MaxHealth(100),
	CurrentHealth(100),
	AttackDamage(0),
	Level(1)
{
	PrimaryActorTick.bCanEverTick = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	CameraComponent->SetupAttachment(SpringArm);

	Collision = GetCapsuleComponent();
	check(Collision);
	Skeletal = GetMesh();
	check(Skeletal);
	MoveCompoment = Cast<UCharacterMovementComponent>(GetMovementComponent());
	check(MoveCompoment);
	MoveCompoment->GravityScale = 1.f;
	MoveCompoment->MaxAcceleration = 2400.0f;
	MoveCompoment->BrakingFrictionFactor = 1.0f;
	MoveCompoment->BrakingFriction = 6.0f;
	MoveCompoment->GroundFriction = 8.0f;
	MoveCompoment->BrakingDecelerationWalking = 1400.0f;
	MoveCompoment->bUseControllerDesiredRotation = false;
	MoveCompoment->bOrientRotationToMovement = false;
	MoveCompoment->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	MoveCompoment->bAllowPhysicsRotationDuringAnimRootMotion = false;
	MoveCompoment->GetNavAgentPropertiesRef().bCanCrouch = true;
	MoveCompoment->bCanWalkOffLedgesWhenCrouching = true;
	MoveCompoment->SetCrouchedHalfHeight(65.0f);
}

FSharedRepMovement::FSharedRepMovement()
{
}

bool FSharedRepMovement::FillForCharacter(ACharacter* Character)
{
	return false;
}

bool FSharedRepMovement::Equals(const FSharedRepMovement& Other, ACharacter* Character) const
{
	return false;
}

bool FSharedRepMovement::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	return false;
}

ABPlayerController* ABCharacter::GetLyraPlayerController() const
{
	return nullptr;
}

ABPlayerState* ABCharacter::GetLyraPlayerState() const
{
	return nullptr;
}

void ABCharacter::BeginPlay()
{
	Super::BeginPlay();
	for (uint32 i = 1; i <= 100; ++i)
	{
		LevelTable.Add(i, (uint32)pow(((i + 2 - 1) * 50 / 49.f), 2.5f));
	}
}

void ABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// TODO Input System ³Ö±â

}

float ABCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	AddCurrentHelth(-DamageAmount);

	if (CurrentHealth <= 0)
	{
		OnDeath();
	}

	return ActualDamage;
}

void ABCharacter::OnDeath()
{
	Destroy();
}

void ABCharacter::AddCurrentHelth(int32 Helth)
{
	CurrentHealth = FMath::Clamp(Helth + CurrentHealth, 0, MaxHealth);
}

void ABCharacter::LevelUP()
{
	++Level;

	if (LevelTable[Level] != 0)
		MaxExperience = LevelTable[Level];

	MaxHealth = Level * 20;
	CurrentHealth = MaxHealth;
}

void ABCharacter::Attack(AActor* Actor)
{
	UGameplayStatics::ApplyDamage(Actor,
		AttackDamage,
		nullptr,
		this,
		UDamageType::StaticClass());
}
void ABCharacter::AddExp(int32 Exp)
{
	if (CurrentExperience + Exp < MaxExperience)
	{
		CurrentExperience += Exp;
		return;
	}
	CurrentExperience += Exp;
	while (CurrentExperience > MaxExperience)
	{
		CurrentExperience -= MaxExperience;
		LevelUP();
	}
}