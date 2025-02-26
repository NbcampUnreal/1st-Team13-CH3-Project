#include "BCharacter.h"
#include "BPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "BMovementComponent.h"

ABCharacter::ABCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UBMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	ActiveWeaponSlot = EWeaponSlot::Primary;  // ê¸°ë³¸ì ìœ¼ë¡œ ì£¼ë¬´ê¸°ë¥¼ í™œì„±í™”
	// ë°°ì—´ì˜ í¬ê¸°ë¥¼ ActiveWeaponSlotì— ë§žê²Œ í™•ìž¥
	EquippedWeapons.SetNumZeroed(4);  // ActiveWeaponSlotì— ë§žê²Œ ë°°ì—´ í¬ê¸° ì„¤ì •
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true; // ?Œì „??ì¹´ë©”?¼ë„ ?´ë™?œë‹¤.
	SpringArm->SetupAttachment(GetRootComponent());

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArm);
	CameraComp->bUsePawnControlRotation = false; // ì¹´ë©”?¼ë„ ?Œì „?˜ë©´ ?•ì‹ ?†ìœ¼ë¯€ë¡?false

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

FVector ABCharacter::GetCameraForwardVector() const
{
	if (CameraComp)
	{
		return CameraComp->GetForwardVector();
	}

	return GetActorForwardVector();  // ì¹´ë©”ë¼ê°€ ì—†ìœ¼ë©´ ê¸°ë³¸ ë°©í–¥ ë°˜í™˜
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
		// PlayerStateë¥?ê°€?¸ì???ì²˜ë¦¬?˜ë„ë¡??˜ìž.
		// TODO : GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ABCharacter::StopSprint(const FInputActionValue& Value)
{
	if (!Value.Get<bool>())
	{
		// PlayerStateë¥?ê°€?¸ì???ì²˜ë¦¬?˜ë„ë¡??˜ìž.
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

void ABCharacter::SetDraggingItem(AActor* NewItem)
{
	ABBaseItem* Item = Cast<ABBaseItem>(NewItem);
	if (Item)
	{
		// ì•„ì´í…œì´ íŠ¹ì • ì†Œì¼“ì— ìž¥ì°©ë˜ì–´ ìžˆë‹¤ë©´ ë“œëž˜ê·¸ ë¶ˆê°€
		if (Item->GetAttachParentSocketName() == "WeaponSocket")
		{
			return;
		}

		DraggingItem = Item;
	}
}

void ABCharacter::StartDragging(const FInputActionValue& Value)
{
	bool Drag = Value.Get<bool>();
	
	if (Drag == true)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, *FString("DragStart"));
	}
}

void ABCharacter::StopDragging(const FInputActionValue& Value)
{
	bool Drag = Value.Get<bool>();

	if (Drag == false)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, *FString("DragStop"));
	}
}

void ABCharacter::AimStop(const FInputActionValue& Value)
{
	if (DraggingItem)
	{
		bIsDragging = true;
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Blue, TEXT("Drag Start"));

		bool bIsValid = GetWorldTimerManager().TimerExists(DragUpdateTimer);
		if (bIsValid)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("Timer Already Exists!"));
		}
		else
		{
			GetWorldTimerManager().SetTimer(DragUpdateTimer, this, &ABCharacter::UpdateDragging, 0.01f, true);
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("Timer Set Successfully!"));
		}
	}
}


void ABCharacter::StopDragging()
{
	if (bIsDragging)
	{
		bIsDragging = false;
		GetWorldTimerManager().ClearTimer(DragUpdateTimer);
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Blue, TEXT("Drag End"));
	}
}


void ABCharacter::UpdateDragging()
{
	if (DraggingItem)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, TEXT("UpdateDragging is Running!"));

		if (!DraggingItem)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("DraggingItem is NULL in UpdateDragging!"));
			return;
		}

		ABPlayerController* PlayerController = Cast<ABPlayerController>(GetController());
		if (PlayerController)
		{

			FVector WorldLocation, WorldDirection;
			if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
			{
				FVector TargetLocation = WorldLocation + WorldDirection * 200.0f;
				// ì•„ì´í…œ ìœ„ì¹˜ ì—…ë°ì´íŠ¸
				DraggingItem->SetActorEnableCollision(false);  // ì¶©ëŒ ë¹„í™œì„±í™”
				DraggingItem->SetActorLocation(TargetLocation);
				DraggingItem->SetActorEnableCollision(true);   // ì´ë™ í›„ ì¶©ëŒ ë‹¤ì‹œ í™œì„±í™”
				bool bMoved = DraggingItem->SetActorLocation(TargetLocation);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("DeprojectMousePositionToWorld Failed!"));
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("UpdateDragging Failed: DraggingItem is NULL!"));
	}
}


void ABCharacter::BeginPlay()
{
	Super::BeginPlay();
}
void ABCharacter::Attack(const struct FInputActionValue& Value)
{
	ABBaseWeapon* CurrentWeapon = EquippedWeapons[(int32)ActiveWeaponSlot];
	if (!CurrentWeapon) return;

	if (CurrentWeapon->WeaponType == "Pistol")
	{
		CurrentWeapon->Attack();
	}
	else if (CurrentWeapon->WeaponType == "Rifle")
	{

		if (!GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ABCharacter::FireOnce, CurrentWeapon->FireRate, true);
		}
	}
	else if (CurrentWeapon->WeaponType == "ShotGun")
	{
		// 권총: 한 번 클릭하면 한 발 발사
		CurrentWeapon->Attack();
	}
	else 
	{
			CurrentWeapon->Attack();
	bool Trigger = Value.Get<bool>();
	if (Trigger == true)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, *FString("Attack"));
	}
}
void ABCharacter::FireOnce()
{
	ABBaseWeapon* CurrentWeapon = EquippedWeapons[(int32)ActiveWeaponSlot];
	if (CurrentWeapon)
	{
		CurrentWeapon->Attack();
	}
}

void ABCharacter::StopFire()
{
	UE_LOG(LogTemp, Log, TEXT("StopFire"));
	// ë°œì‚¬ ë©ˆì¶”ê¸°
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}
// ë¬´ê¸° ë¶€ì°© í•¨ìˆ˜
void ABCharacter::EquipWeapon(ABBaseWeapon* NewWeapon)
{
	if (!NewWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to equip weapon. NewWeapon is null."));
		return;
	}

	USkeletalMeshComponent* CharacterMesh = GetMesh();
	if (!CharacterMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Mesh is null."));
		return;
	}

	// ðŸ”¹ í˜„ìž¬ ì† ì†Œì¼“ì— ìž¥ì°©ëœ ë¬´ê¸° í™•ì¸
	ABBaseWeapon* CurrentWeapon = nullptr;

	// ë¬´ê¸°ê°€ ìž¥ì°©ëœ ì†Œì¼“ì´ ìžˆëŠ”ì§€ í™•ì¸
	for (USceneComponent* ChildComponent : CharacterMesh->GetAttachChildren())
	{
		ABBaseWeapon* AttachedWeapon = Cast<ABBaseWeapon>(ChildComponent->GetOwner());
		if (AttachedWeapon)
		{
			CurrentWeapon = AttachedWeapon;
			break;  // ì²« ë²ˆì§¸ ìž¥ì°©ëœ ë¬´ê¸°ë§Œ ê°€ì ¸ì˜´
		}
	}

	if (!CurrentWeapon)
	{
		// ðŸ”¹ ì†ì— ë¬´ê¸°ê°€ ì—†ìœ¼ë©´ ë°”ë¡œ ìž¥ì°©
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
		NewWeapon->AttachToComponent(CharacterMesh, AttachRules, TEXT("WeaponSocket"));
		UE_LOG(LogTemp, Log, TEXT("WeaponType : %s"), *NewWeapon->WeaponType);

		// ë¬´ê¸° íƒ€ìž…ì— ë”°ë¼ íšŒì „ ì¡°ì •
		if (NewWeapon->WeaponType == "Rifle") {
			FRotator AdjustedRotation(0.0f, -180.0f, 0.0f);
			NewWeapon->SetActorRelativeRotation(AdjustedRotation);
		}
		else if (NewWeapon->WeaponType == "Pistol") {
			FRotator AdjustedRotation(0.0f, 90.0f, 90.0f);
			NewWeapon->SetActorRelativeRotation(AdjustedRotation);
		}
<<<<<<< HEAD
		else if (NewWeapon->WeaponType.Equals("ShotGun", ESearchCase::IgnoreCase)) {
			FRotator AdjustedRotation(0.0f, -180.0f, 0.0f);
			NewWeapon->SetActorRelativeRotation(AdjustedRotation);
		}
		// 무기 정보 설정 및 충돌 처리
=======

		// ë¬´ê¸° ì •ë³´ ì„¤ì • ë° ì¶©ëŒ ì²˜ë¦¬
>>>>>>> b83d57d0c2b4d18c5d67b90f96605927c54b933c
		NewWeapon->SetOwnerCharacter(this);
		NewWeapon->SetActorEnableCollision(false);
		NewWeapon->SetActorHiddenInGame(false);
		// ðŸ”¹ ìž¥ì°©ëœ ë¬´ê¸° ë°°ì—´ì— ì¶”ê°€ (Primary ìŠ¬ë¡¯ì— ìž¥ì°©)
		EquippedWeapons[(int32)EWeaponSlot::Primary] = NewWeapon;
	}
	else
	{
		// ðŸ”¹ ì´ë¯¸ ìž¥ì°©ëœ ë¬´ê¸°ê°€ ìžˆë‹¤ë©´ ë°°ì—´ì— ì €ìž¥ë§Œ í•¨
		EquippedWeapons.Add(NewWeapon);
		// ðŸ”¹ ì¸ë²¤í† ë¦¬ ë§Œë“¤ë©´ ë“¤ì–´ê°ˆ ë¡œì§. ì¼ë‹¨ ë“œëž˜ê·¸ ë˜ê²Œ ë§Œë“¤ì–´ë†“ìŒ
		SetDraggingItem(NewWeapon);
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
	// IA_Look ì•¡ì…˜ ë§ˆìš°ìŠ¤ê°€ "ì›€ì§ì¼ ë•Œ" Look() í˜¸ì¶œ
	EnhancedInput->BindAction(
		PlayerController->AttackAction,
		ETriggerEvent::Triggered,
		this,
		&ABCharacter::Attack
	);
	EnhancedInput->BindAction(
		PlayerController->AttackAction,
		ETriggerEvent::Completed,
		this,
		&ABCharacter::StopFire
	);
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