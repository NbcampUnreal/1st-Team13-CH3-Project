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
	ActiveWeaponSlot = EWeaponSlot::Primary;  // 기본적으로 주무기를 활성화
	// 배열의 크기를 ActiveWeaponSlot에 맞게 확장
	EquippedWeapons.SetNumZeroed(4);  // ActiveWeaponSlot에 맞게 배열 크기 설정
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true; // ȸ���� ī�޶� �̵��Ѵ�.
	SpringArm->SetupAttachment(GetRootComponent());

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArm);
	CameraComp->bUsePawnControlRotation = false; // ī�޶� ȸ���ϸ� ���ž����Ƿ� false

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

	return GetActorForwardVector();  // 카메라가 없으면 기본 방향 반환
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
		// PlayerState�� �����ͼ� ó���ϵ��� ����.
		// TODO : GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ABCharacter::StopSprint(const FInputActionValue& Value)
{
	if (!Value.Get<bool>())
	{
		// PlayerState�� �����ͼ� ó���ϵ��� ����.
		// TODO : GetCharacterMovement()->MaxWalkSpeed = NomalSpeed;
	}
}

void ABCharacter::Reload(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		// TODO : Reload
	}
}

void ABCharacter::SetDraggingItem(AActor* NewItem)
{
	ABBaseItem* Item = Cast<ABBaseItem>(NewItem);
	if (Item)
	{
		// 아이템이 특정 소켓에 장착되어 있다면 드래그 불가
		if (Item->GetAttachParentSocketName() == "WeaponSocket")
		{
			return;
		}

		DraggingItem = Item;
	}
}

void ABCharacter::StartDragging()
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
				// 아이템 위치 업데이트
				DraggingItem->SetActorEnableCollision(false);  // 충돌 비활성화
				DraggingItem->SetActorLocation(TargetLocation);
				DraggingItem->SetActorEnableCollision(true);   // 이동 후 충돌 다시 활성화
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
		// 권총: 한 번 클릭하면 한 발 발사
		CurrentWeapon->Attack();
	}
	else if (CurrentWeapon->WeaponType == "Rifle")
	{

		// 🔹 타이머가 이미 실행 중이면 다시 설정하지 않음
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
		// 투척무기: 투척
			CurrentWeapon->Attack();
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
	// 발사 멈추기
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}
// 무기 부착 함수
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

	// 🔹 현재 손 소켓에 장착된 무기 확인
	ABBaseWeapon* CurrentWeapon = nullptr;

	// 무기가 장착된 소켓이 있는지 확인
	for (USceneComponent* ChildComponent : CharacterMesh->GetAttachChildren())
	{
		ABBaseWeapon* AttachedWeapon = Cast<ABBaseWeapon>(ChildComponent->GetOwner());
		if (AttachedWeapon)
		{
			CurrentWeapon = AttachedWeapon;
			break;  // 첫 번째 장착된 무기만 가져옴
		}
	}

	if (!CurrentWeapon)
	{
		// 🔹 손에 무기가 없으면 바로 장착
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
		NewWeapon->AttachToComponent(CharacterMesh, AttachRules, TEXT("WeaponSocket"));
		UE_LOG(LogTemp, Log, TEXT("WeaponType : %s"), *NewWeapon->WeaponType);

		// 무기 타입에 따라 회전 조정
		if (NewWeapon->WeaponType == "Rifle") {
			FRotator AdjustedRotation(0.0f, -180.0f, 0.0f);
			NewWeapon->SetActorRelativeRotation(AdjustedRotation);
		}
		else if (NewWeapon->WeaponType == "Pistol") {
			FRotator AdjustedRotation(0.0f, 90.0f, 90.0f);
			NewWeapon->SetActorRelativeRotation(AdjustedRotation);
		}
		else if (NewWeapon->WeaponType.Equals("ShotGun", ESearchCase::IgnoreCase)) {
			FRotator AdjustedRotation(0.0f, -180.0f, 0.0f);
			NewWeapon->SetActorRelativeRotation(AdjustedRotation);
		}
		// 무기 정보 설정 및 충돌 처리
		NewWeapon->SetOwnerCharacter(this);
		NewWeapon->SetActorEnableCollision(false);
		NewWeapon->SetActorHiddenInGame(false);
		// 🔹 장착된 무기 배열에 추가 (Primary 슬롯에 장착)
		EquippedWeapons[(int32)EWeaponSlot::Primary] = NewWeapon;
	}
	else
	{
		// 🔹 이미 장착된 무기가 있다면 배열에 저장만 함
		EquippedWeapons.Add(NewWeapon);
		// 🔹 인벤토리 만들면 들어갈 로직. 일단 드래그 되게 만들어놓음
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
	// IA_Look 액션 마우스가 "움직일 때" Look() 호출
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
}