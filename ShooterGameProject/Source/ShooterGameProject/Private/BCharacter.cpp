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
#include "Components/SphereComponent.h"
#include "BBaseItem.h"
#include "BPlayerState.h"
#include "BUIManager.h"
#include "BGameInstance.h"
#include "BInventoryWidget.h"
#include <BShotGun.h>

ABCharacter::ABCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UBMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	ActiveWeaponSlot = EWeaponSlot::Pistol;  // ê¸°ë³¸ì ìœ¼ë¡œ ì£¼ë¬´ê¸°ë¥¼ í™œì„±í™”
	// ë°°ì—´ì˜ í¬ê¸°ë¥¼ ActiveWeaponSlotì— ë§žê²Œ í™•ìž¥
	EquippedWeapons.SetNumZeroed(5);  // ActiveWeaponSlotì— ë§žê²Œ ë°°ì—´ í¬ê¸° ì„¤ì •
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

	CollectNearItem = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollectNearItem->SetupAttachment(GetRootComponent());
	CollectNearItem->SetSphereRadius(400.f);
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
void ABCharacter::AimStart(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, *FString("AimStart"));
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

void ABCharacter::ZoomStart(const FInputActionValue& Value)
{
	if (CameraComp)
	{
		CameraComp->SetFieldOfView(ZoomedFOV); // 줌 상태 (예: 45.0f)
	}
}

void ABCharacter::ZoomStop(const FInputActionValue& Value)
{
	if (CameraComp)
	{
		CameraComp->SetFieldOfView(DefaultFOV); // 원래 상태 (예: 90.0f)
	}
}

void ABCharacter::BeginPlay()
{
	Super::BeginPlay();
}
void ABCharacter::Attack(const struct FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Attack() called"));
	if (EquippedWeapon == nullptr) 
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("⚔️ 공격 시작: %s"), *EquippedWeapon->GetName());
	
	// 🔹 유효한 슬롯인지 확인
	if (!EquippedWeapons.IsValidIndex((int32)ActiveWeaponSlot))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid ActiveWeaponSlot: %d"), (int32)ActiveWeaponSlot);
		return;
	}

	// 🔹 현재 슬롯의 무기 가져오기
	ABBaseWeapon* CurrentWeapon = EquippedWeapons[(int32)ActiveWeaponSlot];
	if (!CurrentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("No weapon equipped in slot: %d"), (int32)ActiveWeaponSlot);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("🔍 [FireOnce] 현재 무기 타입: %s"), *CurrentWeapon->WeaponType);

	
	CurrentWeapon->Attack();
}


void ABCharacter::StopFire()
{
	UE_LOG(LogTemp, Log, TEXT("StopFire"));
	// ë°œì‚¬ ë©ˆì¶”ê¸°
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}

void ABCharacter::PickupWeapon(ABBaseWeapon* NewWeapon)
{
	if (!NewWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("❌ Failed to pick up weapon. NewWeapon is null."));
		return;
	}

	// 🔹 이미 인벤토리에 있는지 확인
	for (ABBaseWeapon* ExistingWeapon : EquippedWeapons)
	{
		if (ExistingWeapon == NewWeapon)  // 같은 무기인지 확인
		{
			return;  // 중복 추가 방지
		}
	}

	// 🔹 무기 배열에 추가 (배열 크기 체크)
	if (NewWeapon->WeaponType.Equals("Melee", ESearchCase::IgnoreCase))
	{
		EquippedWeapons[(int32)EWeaponSlot::Melee] = NewWeapon;
	}
	else if (NewWeapon->WeaponType.Equals("Grenade", ESearchCase::IgnoreCase))
	{
		EquippedWeapons[(int32)EWeaponSlot::Throwable] = NewWeapon;
	}
	else if (NewWeapon->WeaponType.Equals("Rifle", ESearchCase::IgnoreCase))
	{
		EquippedWeapons[(int32)EWeaponSlot::Rifle] = NewWeapon;
	}
	else if (NewWeapon->WeaponType.Equals("Pistol", ESearchCase::IgnoreCase))
	{
		EquippedWeapons[(int32)EWeaponSlot::Pistol] = NewWeapon;
	}
	else if (NewWeapon->WeaponType.Equals("ShotGun", ESearchCase::IgnoreCase))
	{
		EquippedWeapons[(int32)EWeaponSlot::ShotGun] = NewWeapon;
	}

	// ✅ 여기서 바로 WeaponSocket으로 부착하지 않음
	FName StorageSocketName = TEXT("WeaponStorageSocket");
	if (GetMesh()->DoesSocketExist(StorageSocketName))
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, StorageSocketName);
		UE_LOG(LogTemp, Log, TEXT("✅ Weapon stored in %s"), *StorageSocketName.ToString());
		NewWeapon->SetActorHiddenInGame(true);
	}

	UE_LOG(LogTemp, Log, TEXT("✅ Picked up WeaponType: %s"), *NewWeapon->WeaponType);
}

ABBaseWeapon* ABCharacter::GetCurrentWeapon() const
{
	return EquippedWeapon;
}



void ABCharacter::EquipWeaponByType(EWeaponSlot Slot)
{
	// 🔹 인덱스가 올바른지 검사
	if (!EquippedWeapons.IsValidIndex((int32)Slot))
	{
		UE_LOG(LogTemp, Error, TEXT("EquipWeaponByType: Slot %d is out of bounds! Max size: %d"),
			(int32)Slot, EquippedWeapons.Num());
		return;
	}

	// 🔹 무기 배열 크기가 부족하면 확장
	if (EquippedWeapons.Num() <= (int32)Slot)
	{
		EquippedWeapons.SetNum((int32)Slot + 1);
	}

	// 🔹 현재 슬롯 업데이트
	ActiveWeaponSlot = Slot;
	ABBaseWeapon* WeaponToEquip = EquippedWeapons[(int32)Slot];

	if (!WeaponToEquip)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ WeaponToEquip is nullptr! Slot: %d"), (int32)Slot);
		return;
	}

	// 🔹 타이머 초기화 (발사 딜레이 리셋)
	GetWorldTimerManager().ClearTimer(FireTimerHandle);

	// 🔹 무기 메쉬 확인
	UStaticMeshComponent* WeaponMesh = WeaponToEquip->FindComponentByClass<UStaticMeshComponent>();
	if (!WeaponMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ WeaponMesh is nullptr for %s"), *WeaponToEquip->GetName());
		return;
	}

	// 🔹 기존 장착 무기 숨기기 + 인벤토리에 저장
	if (EquippedWeapon)
	{
		UE_LOG(LogTemp, Log, TEXT("Hiding previously equipped weapon: %s"), *EquippedWeapon->WeaponType);
		EquippedWeapon->SetActorHiddenInGame(true);
		EquippedWeapon->SetActorEnableCollision(false);
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);  // ✅ 맵에 남아있는 문제 해결

		// 무기 보관 위치 설정
		FName StorageSocketName = TEXT("WeaponBackSocket");
		if (EquippedWeapon->WeaponType == "Pistol")
		{
			StorageSocketName = TEXT("PistolHolsterSocket");
		}
		else if (EquippedWeapon->WeaponType == "Shotgun")
		{
			StorageSocketName = TEXT("ShotgunBackSocket");
		}
		else if (EquippedWeapon->WeaponType == "Rifle")
		{
			StorageSocketName = TEXT("RifleBackSocket");
		}
		else if (EquippedWeapon->WeaponType == "Melee")
		{
			StorageSocketName = TEXT("MeleeSocket");
		}
		if (GetMesh()->DoesSocketExist(StorageSocketName))
		{
			EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, StorageSocketName);
		}
	}

	// 🔹 새로운 무기 장착
	FName TargetSocketName = TEXT("WeaponSocket");
	if (GetMesh()->DoesSocketExist(TargetSocketName))
	{
		// 🔹 먼저 무기를 숨김 해제
		WeaponToEquip->SetActorHiddenInGame(false);
		WeaponToEquip->SetActorEnableCollision(true);
		WeaponToEquip->ForceNetUpdate(); // 네트워크 동기화

		// 🔹 무기 부착
		WeaponToEquip->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TargetSocketName);

		// 🔹 무기 메쉬 처리
		if (WeaponMesh)
		{
			WeaponMesh->SetHiddenInGame(false);
			WeaponMesh->SetVisibility(true);
			WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // ✅ 무기 충돌 제거
		}

		// 🔹 무기 회전값 조정 (무기 타입별)
		FRotator AdjustedRotation(0.0f, 0.0f, 0.0f);
		if (WeaponToEquip->WeaponType == "Rifle" || WeaponToEquip->WeaponType == "Shotgun")
		{
			AdjustedRotation = FRotator(0.0f, -180.0f, 0.0f);
		}
		else if (WeaponToEquip->WeaponType == "Pistol")
		{
			AdjustedRotation = FRotator(0.0f, 90.0f, 90.0f);
		}
		WeaponToEquip->SetActorRelativeRotation(AdjustedRotation);

		// 🔹 장착된 무기 업데이트
		EquippedWeapon = WeaponToEquip;

		UE_LOG(LogTemp, Warning, TEXT("✅ Equipped %s on %s"), *EquippedWeapon->WeaponType, *TargetSocketName.ToString());
		UE_LOG(LogTemp, Warning, TEXT("📌 CurrentWeapon: %s"), *EquippedWeapon->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon socket %s does not exist!"), *TargetSocketName.ToString());
	}
}

void ABCharacter::InventorySwitch()
{
	static bool Switch = false;

	if (!Switch)
	{
		ShowInventory();
	}
	else
	{
		CloseInventory();
	}
	Switch = !Switch;
}

void ABCharacter::EquipPistol()
{
	UE_LOG(LogTemp, Warning, TEXT("EquipPistol() called!"));
	ActiveWeaponSlot = EWeaponSlot::Pistol;
	EquipWeaponByType(ActiveWeaponSlot);
}

void ABCharacter::EquipRifle()
{
	UE_LOG(LogTemp, Warning, TEXT("EquipRifle() called!"));
	ActiveWeaponSlot = EWeaponSlot::Rifle;
	EquipWeaponByType(ActiveWeaponSlot);
}

void ABCharacter::EquipShotgun()
{
	UE_LOG(LogTemp, Warning, TEXT("EquipShotgun() called!"));
	ActiveWeaponSlot = EWeaponSlot::ShotGun;
	EquipWeaponByType(ActiveWeaponSlot);
}

void ABCharacter::EquipMelee()
{
	UE_LOG(LogTemp, Warning, TEXT("EquipMelee() called!"));
	ActiveWeaponSlot = EWeaponSlot::Melee;
	EquipWeaponByType(ActiveWeaponSlot);
}

TArray<ABBaseItem*> ABCharacter::GetNearItemArray() const
{
	TArray<AActor*> ActivateItem;
	TArray<ABBaseItem*> BaseItem;

	CollectNearItem->GetOverlappingActors(ActivateItem);

	for (AActor* Actor : ActivateItem)
	{
		if (ABBaseItem* Base = Cast<ABBaseItem>(Actor))
		{
			BaseItem.Add(Base);
		}
	}
	return BaseItem;
}

void ABCharacter::ShowInventory()
{
	if (UBGameInstance* Instance = Cast<UBGameInstance>(GetGameInstance()))
	{
		if (UBUIManager* UIManager = Cast<UBUIManager>(Instance->GetUIManagerInstance()))
		{
			UIManager->ShowInventory();
			if (UBInventoryWidget* Inventory = Cast<UBInventoryWidget>(UIManager->GetInventoryInstance()))
			{
				Inventory->SendItemData(GetNearItemArray(), Cast<ABPlayerState>(GetPlayerState()));
			}
		}
	}
}

void ABCharacter::CloseInventory()
{
	if (UBGameInstance* Instance = Cast<UBGameInstance>(GetGameInstance()))
	{
		if (UBUIManager* UIManager = Cast<UBUIManager>(Instance->GetUIManagerInstance()))
		{
			UIManager->CloseInventory();
		}
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
		&ABCharacter::Attack
	);
	
	EnhancedInput->BindAction(
		PlayerController->ReloadAction,
		ETriggerEvent::Completed,
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
	EnhancedInput->BindAction(
		PlayerController->EquipPistolAction,
		ETriggerEvent::Completed,  // 🔹 키를 누르는 순간 실행되도록 변경
		this,
		&ABCharacter::EquipPistol);
	EnhancedInput->BindAction(
		PlayerController->EquipRifleAction,
		ETriggerEvent::Completed,  // 🔹 키를 누르는 순간 실행
		this,
		&ABCharacter::EquipRifle);
	EnhancedInput->BindAction(
		PlayerController->EquipShotgunAction,
		ETriggerEvent::Completed,  // 🔹 키를 누르는 순간 실행
		this,
		&ABCharacter::EquipShotgun);
	EnhancedInput->BindAction(
		PlayerController->EquipMeleeAction,
		ETriggerEvent::Completed,  // 🔹 키를 누르는 순간 실행
		this,
		&ABCharacter::EquipMelee);
	EnhancedInput->BindAction(
		PlayerController->ZoomAction,
		ETriggerEvent::Triggered,
		this,
		&ABCharacter::ZoomStart);
	EnhancedInput->BindAction(
		PlayerController->ZoomAction,
		ETriggerEvent::Completed,
		this,
		&ABCharacter::ZoomStop);
}