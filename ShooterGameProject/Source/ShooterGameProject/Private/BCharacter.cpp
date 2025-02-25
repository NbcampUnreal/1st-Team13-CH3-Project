#include "BCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BPlayerController.h"
#include "EnhancedInputComponent.h"
#include "BMovementComponent.h"

ABCharacter::ABCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UBMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true; // ?åÏ†Ñ??Ïπ¥Î©î?ºÎèÑ ?¥Îèô?úÎã§.
	SpringArm->SetupAttachment(GetRootComponent());

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArm);
	CameraComp->bUsePawnControlRotation = false; // Ïπ¥Î©î?ºÎèÑ ?åÏ†Ñ?òÎ©¥ ?ïÏã†?ÜÏúºÎØÄÎ°?false

	Collision = GetCapsuleComponent();
	check(Collision);
	Skeletal = GetMesh();
	check(Skeletal);	
	MoveComp = CastChecked<UBMovementComponent>(GetCharacterMovement());
	check(MoveComp);
	MoveComp->GravityScale = 1.f;
	MoveComp->MaxAcceleration = 2400.0f;
	MoveComp->BrakingFrictionFactor = 1.0f;
	MoveComp->BrakingFriction = 6.0f;
	MoveComp->GroundFriction = 8.0f;
	MoveComp->BrakingDecelerationWalking = 1400.0f;
	MoveComp->bUseControllerDesiredRotation = false;
	MoveComp->bOrientRotationToMovement = false;
	MoveComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	MoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
	MoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	MoveComp->bCanWalkOffLedgesWhenCrouching = true;
	MoveComp->SetCrouchedHalfHeight(65.0f);
}

ABPlayerState* ABCharacter::GetBPlayerState() const
{
	return CastChecked<ABPlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

ABPlayerController* ABCharacter::GetBPlayerController() const
{
	return CastChecked<ABPlayerController>(Controller, ECastCheckedType::NullAllowed);
}

void ABCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;

	FVector2D MoveInput = Value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void ABCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookInput = Value.Get<FVector2D>();
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ABCharacter::StartJump(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		Jump();
	}
}

void ABCharacter::StopJump(const FInputActionValue& Value)
{
	if (!Value.Get<bool>())
	{
		StopJumping();
	}
}

void ABCharacter::StartSprint(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		// PlayerStateÎ•?Í∞Ä?∏Ï???Ï≤òÎ¶¨?òÎèÑÎ°??òÏûê.
		// TODO : GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ABCharacter::StopSprint(const FInputActionValue& Value)
{
	if (!Value.Get<bool>())
	{
		// PlayerStateÎ•?Í∞Ä?∏Ï???Ï≤òÎ¶¨?òÎèÑÎ°??òÏûê.
		// TODO : GetCharacterMovement()->MaxWalkSpeed = NomalSpeed;
	}
}

void ABCharacter::Reload(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, *FString("Reload"));
	}
}

void ABCharacter::AimStart(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, *FString("AimStart"));
	}
}

void ABCharacter::AimStop(const FInputActionValue& Value)
{
	if (!Value.Get<bool>())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f,FColor::Blue, *FString("AimStop"));
	}
}

void ABCharacter::BeginPlay()
{
	Super::BeginPlay();
}
void ABCharacter::Attack(const struct FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		// TODO :: Fire
	}
}
void ABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInput == nullptr)
	{
		return;
	}
	ABPlayerController* PlayerController = Cast<ABPlayerController>(GetController());
	if (PlayerController == nullptr)
	{
		return;
	}

	EnhancedInput->BindAction(
		PlayerController->MoveAction,
		ETriggerEvent::Triggered,
		this,
		&ABCharacter::Move);
	EnhancedInput->BindAction(
		PlayerController->SprintAction,
		ETriggerEvent::Triggered,
		this,
		&ABCharacter::StartSprint);
	EnhancedInput->BindAction(
		PlayerController->SprintAction,
		ETriggerEvent::Completed,
		this,
		&ABCharacter::StopSprint);

	EnhancedInput->BindAction(
		PlayerController->LookAction,
		ETriggerEvent::Triggered,
		this,
		&ABCharacter::Look);

	EnhancedInput->BindAction(
		PlayerController->JumpAction,
		ETriggerEvent::Triggered,
		this,
		&ABCharacter::StartJump);
	EnhancedInput->BindAction(
		PlayerController->JumpAction,
		ETriggerEvent::Completed,
		this,
		&ABCharacter::StopJump);
	EnhancedInput->BindAction(
		PlayerController->AttackAction,
		ETriggerEvent::Triggered,
		this,
		&ABCharacter::Attack);
	EnhancedInput->BindAction(
		PlayerController->ReloadAction,
		ETriggerEvent::Triggered,
		this,
		&ABCharacter::Reload);
	EnhancedInput->BindAction(
		PlayerController->DragAction,  
		ETriggerEvent::Triggered, 
		this, 
		&ABCharacter::StartDragging);
	EnhancedInput->BindAction(
		PlayerController->DragAction, 
		ETriggerEvent::Completed, 
		this, 
		&ABCharacter::StopDragging);
	EnhancedInput->BindAction(
		PlayerController->AimAction,
		ETriggerEvent::Triggered,
		this,
		&ABCharacter::AimStart);
	EnhancedInput->BindAction(
		PlayerController->AimAction,
		ETriggerEvent::Completed,
		this,
		&ABCharacter::AimStop);
}