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
#include "BBaseGun.h"
#include "BRiflePart.h"
#include "BPistolPart.h"
#include "BShotgunPart.h"
#include "BPlayerState.h"
#include "BUIManager.h"
#include "BGameInstance.h"
#include "BInventoryWidget.h"



ABCharacter::ABCharacter(const FObjectInitializer& ObjectInitializer)
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
	if (!EquippedWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("장착된 무기가 없습니다. 리로드 실패."));
		return;
	}

	if (ABBaseGun* CurrentGun = Cast<ABBaseGun>(EquippedWeapon))
	{
		// 탄창이 필요한 무기일 경우 (예: 라이플, 샷건 등)
		if (CurrentGun->WeaponType == "Rifle")
		{
			// 리로드 시작
			UE_LOG(LogTemp, Log, TEXT("라이플 리로드 중..."));

				if (ABPlayerState* BPlayerState = GetBPlayerState())
				{
					FName RifleMagazine = "RifleMagazine";
					// 무기의 종류에 맞는 탄약 아이템을 인벤토리에서 찾음
					TArray<FItemData> AmmoItems = BPlayerState->GetInventoryTypeItem(RifleMagazine);
					if (AmmoItems.Num() > 0)
					{
						// 아이템이 있다면 첫 번째 아이템을 사용하여 리로드
						ABBaseItem* AmmoItem = AmmoItems[0].ItemRef;  // FItemData 내에서 ItemRef를 통해 인스턴스 가져오기
						if (AmmoItem)
						{
							AmmoItem->UseItem(this);  // UseItem을 호출하여 리로드 실행
							UE_LOG(LogTemp, Log, TEXT("리로드 아이템을 사용하여 탄약 추가"));
							// UseItem이 끝난 후 리로드를 호출하여 총기의 탄약 상태 업데이트
							CurrentGun->Reload(); // 리로드 함수 호출하여 실제 총기의 CurrentAmmo 증가
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("리로드 아이템을 찾을 수 없습니다."));
					}
				}
		}
		// 탄창이 필요한 무기일 경우 (예: 라이플, 샷건 등)
		else if (CurrentGun->WeaponType == "ShotGun")
		{
			// 리로드 시작
			UE_LOG(LogTemp, Log, TEXT("샷건 리로드 중..."));

			if (ABPlayerState* BPlayerState = GetBPlayerState())
				{
					FName ShotGunMagazine = "ShotgunMagazine";
					// 무기의 종류에 맞는 탄약 아이템을 인벤토리에서 찾음
					TArray<FItemData> AmmoItems = BPlayerState->GetInventoryTypeItem(ShotGunMagazine);
					if (AmmoItems.Num() > 0)
					{
						// 아이템이 있다면 첫 번째 아이템을 사용하여 리로드
						ABBaseItem* AmmoItem = AmmoItems[0].ItemRef;  // FItemData 내에서 ItemRef를 통해 인스턴스 가져오기
						if (AmmoItem)
						{
							AmmoItem->UseItem(this);  // UseItem을 호출하여 리로드 실행
							UE_LOG(LogTemp, Log, TEXT("리로드 아이템을 사용하여 탄약 추가"));
							// UseItem이 끝난 후 리로드를 호출하여 총기의 탄약 상태 업데이트
							CurrentGun->Reload(); // 리로드 함수 호출하여 실제 총기의 CurrentAmmo 증가
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("리로드 아이템을 찾을 수 없습니다."));
					}
				}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("리로드할 수 없는 무기입니다."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("현재 장착된 무기가 총기가 아닙니다."));
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

	State = Cast<ABPlayerState>(GetPlayerState());
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
	if (CurrentWeapon->WeaponType == "Grenade" && GrenadeCount <= 0) 
	{
		UE_LOG(LogTemp, Log, TEXT("💣 수류탄 장착 해제 완료!"));
		UnequipGrenade();
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("🔍 [FireOnce] 현재 무기 타입: %s"), *CurrentWeapon->WeaponType);

	CurrentWeapon->Attack();
}
void ABCharacter::UnequipGrenade()
{
	if (EquippedWeapon && EquippedWeapon->WeaponType == "Grenade")
	{
			UE_LOG(LogTemp, Log, TEXT("Hiding previously equipped weapon: %s"), *EquippedWeapon->WeaponType);
			EquippedWeapon->SetActorHiddenInGame(true);
			EquippedWeapon->SetActorEnableCollision(false);
			EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

			// 무기 보관 위치 설정
			FName StorageSocketName = TEXT("WeaponStorageSocket");
			if (GetMesh()->DoesSocketExist(StorageSocketName))
			{
				EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, StorageSocketName);
			}
		
		UE_LOG(LogTemp, Log, TEXT("💣 수류탄 장착 해제 완료!"));
	}
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
		GrenadeCount++;
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
		if (NewWeapon->WeaponType.Equals("Grenade", ESearchCase::IgnoreCase))
		{
			// 수류탄의 스케일을 줄여서 설정 (예: 0.5배)
			NewWeapon->SetActorScale3D(FVector(0.05f, 0.05f, 0.05f));

			UE_LOG(LogTemp, Log, TEXT("Grenade picked up. Scale set to 0.05."));
		}
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

	// 🔹 수류탄인데 개수가 0이면 장착 안되게 설정
	if (Slot == EWeaponSlot::Throwable && GrenadeCount <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ GrenadeCount is 0"));
		WeaponMesh = nullptr;
	}

	// 🔹 WeaponMesh가 nullptr이면 기존 무기 해제
	if (!WeaponMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("❌ WeaponMesh is nullptr, unequipping current weapon"));
		if (EquippedWeapon)
		{
			EquippedWeapon->SetActorHiddenInGame(true);
			EquippedWeapon->SetActorEnableCollision(false);
			EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			EquippedWeapon = nullptr;
		}
		return;
	}

	// 🔹 기존 장착 무기 숨기기 + 인벤토리에 저장
	if (EquippedWeapon)
	{
		UE_LOG(LogTemp, Log, TEXT("Hiding previously equipped weapon: %s"), *EquippedWeapon->WeaponType);
		EquippedWeapon->SetActorHiddenInGame(true);
		EquippedWeapon->SetActorEnableCollision(false);
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		// 무기 보관 위치 설정
		FName StorageSocketName = TEXT("WeaponStorageSocket");
		if (GetMesh()->DoesSocketExist(StorageSocketName))
		{
			EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, StorageSocketName);
		}
	}

	// 🔹 새로운 무기 장착
	FName TargetSocketName = TEXT("WeaponSocket");
	if (!GetMesh()->DoesSocketExist(TargetSocketName))
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Target Socket %s does not exist!"), *TargetSocketName.ToString());
		return;
	}

	// 🔹 먼저 무기를 숨김 해제
	WeaponToEquip->SetActorHiddenInGame(false);
	WeaponToEquip->SetActorEnableCollision(true);
	WeaponToEquip->ForceNetUpdate(); // 네트워크 동기화

	// 🔹 AttachToComponent 시도 및 성공 여부 확인
	bool bAttachSuccess = WeaponToEquip->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, TargetSocketName);

	if (!bAttachSuccess)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ AttachToComponent FAILED for %s!"), *WeaponToEquip->GetName());
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("✅ %s attached to %s successfully: %s"), *WeaponToEquip->GetName(), *TargetSocketName.ToString(), *WeaponToEquip -> GetActorLocation().ToString());
	}

	// 🔹 무기 메쉬 처리
	if (WeaponMesh)
	{
		WeaponMesh->SetHiddenInGame(false);
		WeaponMesh->SetVisibility(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// 🔹 물리(Physics) 영향 방지
	WeaponMesh->SetSimulatePhysics(false);

	// 🔹 무기 회전값 조정 (무기 타입별)
	FRotator AdjustedRotation(0.0f, 0.0f, 0.0f);
	if (WeaponToEquip->WeaponType == "Rifle")
	{
		AdjustedRotation = FRotator(0.0f, -180.0f, 0.0f);
	}
	else if (WeaponToEquip->WeaponType == "Shotgun") 
	{
		AdjustedRotation = FRotator(0.0f, -180.0f, 0.0f);
	}
	else if (WeaponToEquip->WeaponType == "Pistol")
	{
		AdjustedRotation = FRotator(0.0f, 90.0f, 90.0f);
	}
	else if (WeaponToEquip->WeaponType == "Melee")
	{
		AdjustedRotation = FRotator(90.0f, -90.0f, 90.0f);
	}
	else if (WeaponToEquip->WeaponType == "Grenade")
	{
		WeaponToEquip->SetActorRelativeLocation(FVector::ZeroVector);
		AdjustedRotation = FRotator(90.0f, -90.0f, 90.0f); 
	}
	WeaponToEquip->SetActorRelativeRotation(AdjustedRotation);

	// 🔹 장착된 무기 업데이트
	EquippedWeapon = WeaponToEquip;

	UE_LOG(LogTemp, Warning, TEXT("📌 CurrentWeapon: %s"), *EquippedWeapon->GetName());

	if (EquippedWeapon->WeaponType.Equals("Rifle"))  // Equals() 사용
	{
		UE_LOG(LogTemp, Warning, TEXT("라이플 파츠 장착"));
		EquipRifleParts();  // 라이플 파츠 장착
		if (UBGameInstance* Instance = Cast<UBGameInstance>(GetGameInstance()))
		{
			if (UBUIManager* UIManager = Cast<UBUIManager>(Instance->GetUIManagerInstance()))
			{
				if (ABBaseGun* Gun = Cast<ABBaseGun>(EquippedWeapon))
				{
					
					FString Message = TEXT("데미지: ") + FString::Printf(TEXT("%d"), Gun->WeaponDamage) +
						TEXT(", 발사속도: ") + FString::Printf(TEXT("%.0f"), Gun->FireRate) +
						TEXT(", 최대 장탄 수: ") + FString::Printf(TEXT("%d"), Gun->MaxAmmo);
					UIManager->DisplayNotification(TEXT("라이플 강화 완료"), Message);
				}
				if (UBInventoryWidget* Inventory = Cast<UBInventoryWidget>(UIManager->GetInventoryInstance()))
				{
					Inventory->SendItemData(GetNearItemArray(), Cast<ABPlayerState>(GetPlayerState()));
				}
			}
		}
	}
	else if (EquippedWeapon->WeaponType.Equals("Shotgun"))  // Equals() 사용
	{
		UE_LOG(LogTemp, Warning, TEXT("샷건 파츠 장착"));
		EquipShotgunParts();  // 라이플 파츠 장착
		if (UBGameInstance* Instance = Cast<UBGameInstance>(GetGameInstance()))
		{
			if (UBUIManager* UIManager = Cast<UBUIManager>(Instance->GetUIManagerInstance()))
			{
				if (ABBaseGun* Gun = Cast<ABBaseGun>(EquippedWeapon))
				{
					FString Message = TEXT("데미지: ") + FString::Printf(TEXT("%d"), Gun->WeaponDamage) +
						TEXT(", 발사속도: ") + FString::Printf(TEXT("%.0f"), Gun->FireRate) +
						TEXT(", 최대 장탄 수: ") + FString::Printf(TEXT("%d"), Gun->MaxAmmo);
					UIManager->DisplayNotification(TEXT("샷건 강화 완료"), Message);
				}
				if (UBInventoryWidget* Inventory = Cast<UBInventoryWidget>(UIManager->GetInventoryInstance()))
				{
					Inventory->SendItemData(GetNearItemArray(), Cast<ABPlayerState>(GetPlayerState()));
				}
			}
		}
	}
	else if (EquippedWeapon->WeaponType.Equals("Pistol"))  // Equals() 사용
	{
		UE_LOG(LogTemp, Warning, TEXT("피스톨 파츠 장착"));
		if (UBGameInstance* Instance = Cast<UBGameInstance>(GetGameInstance()))
		{
			if (UBUIManager* UIManager = Cast<UBUIManager>(Instance->GetUIManagerInstance()))
			{
				if (ABBaseGun* Gun = Cast<ABBaseGun>(EquippedWeapon))
				{
					FString Message = TEXT("데미지: ") + FString::Printf(TEXT("%d"), Gun->WeaponDamage);
					UIManager->DisplayNotification(TEXT("피스톨 강화 완료"), Message);
				}
				if (UBInventoryWidget* Inventory = Cast<UBInventoryWidget>(UIManager->GetInventoryInstance()))
				{
					Inventory->SendItemData(GetNearItemArray(), Cast<ABPlayerState>(GetPlayerState()));
				}
			}
		}
		EquipPistolParts();  // 라이플 파츠 장착
	}
}
void ABCharacter::EquipRifleParts()
{
	ABPlayerState* BPlayerState = GetBPlayerState();
	if (!BPlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어 상태를 찾을 수 없습니다."));
		return;
	}

	// RiflePart 클래스로 인벤토리 검색
	TArray<FItemData> RiflePartsInInventory = BPlayerState->GetInventoryClassItem(ABRiflePart::StaticClass());
	UE_LOG(LogTemp, Log, TEXT("Found %d RiflePart items in inventory."), RiflePartsInInventory.Num());

	// 장착 후 제거할 아이템 목록
	TArray<FItemData> ItemsToRemove;

	for (const FItemData& RiflePartItem : RiflePartsInInventory)
	{
		if (RiflePartItem.ItemClass)
		{
			// ItemClass에서 RiflePart 객체 생성
			ABRiflePart* RiflePart = NewObject<ABRiflePart>(this, RiflePartItem.ItemClass);

			if (RiflePart)
			{
				UE_LOG(LogTemp, Log, TEXT("Attempting to apply RiflePart: %s"), *RiflePart->PartName);

				// RiflePart를 라이플에 장착
				RiflePart->UseItem(this);

				// 제거할 아이템 목록에 추가 (바로 삭제하면 안 됨)
				ItemsToRemove.Add(RiflePartItem);

				UE_LOG(LogTemp, Log, TEXT("%s 파츠가 라이플에 장착되었습니다."), *RiflePart->PartName);
				
				
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("라이플 파츠를 찾을 수 없습니다."));
			}
		}
	}

	// 🔹 모든 아이템을 루프가 끝난 후 삭제
	for (const FItemData& Item : ItemsToRemove)
	{
		BPlayerState->InventoryRemoveItem(Item);
	}
	
}

void ABCharacter::EquipPistolParts()
{
	ABPlayerState* BPlayerState = GetBPlayerState();
	if (!BPlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어 상태를 찾을 수 없습니다."));
		return;
	}

	// PistolPart 클래스로 인벤토리 검색
	TArray<FItemData> PistolPartsInInventory = BPlayerState->GetInventoryClassItem(ABPistolPart::StaticClass());
	UE_LOG(LogTemp, Log, TEXT("Found %d PistolPart items in inventory."), PistolPartsInInventory.Num());

	// 장착 후 제거할 아이템 목록
	TArray<FItemData> ItemsToRemove;

	for (const FItemData& PistolPartItem : PistolPartsInInventory)
	{
		if (PistolPartItem.ItemClass)
		{
			// ItemClass에서 PistolPart 객체 생성
			ABPistolPart* PistolPart = NewObject<ABPistolPart>(this, PistolPartItem.ItemClass);

			if (PistolPart)
			{
				UE_LOG(LogTemp, Log, TEXT("Attempting to apply PistolPart: %s"), *PistolPart->PartName);

				// PistolPart를 피스톨에 장착
				PistolPart->UseItem(this);

				// 제거할 아이템 목록에 추가
				ItemsToRemove.Add(PistolPartItem);

				UE_LOG(LogTemp, Log, TEXT("%s 파츠가 피스톨에 장착되었습니다."), *PistolPart->PartName);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("피스톨 파츠를 찾을 수 없습니다."));
			}
		}
	}

	// 🔹 루프 종료 후 아이템 삭제
	for (const FItemData& Item : ItemsToRemove)
	{
		BPlayerState->InventoryRemoveItem(Item);
	}
}

void ABCharacter::EquipShotgunParts()
{
	ABPlayerState* BPlayerState = GetBPlayerState();
	if (!BPlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어 상태를 찾을 수 없습니다."));
		return;
	}

	// ShotgunPart 클래스로 인벤토리 검색
	TArray<FItemData> ShotgunPartsInInventory = BPlayerState->GetInventoryClassItem(ABShotgunPart::StaticClass());
	UE_LOG(LogTemp, Log, TEXT("Found %d ShotgunPart items in inventory."), ShotgunPartsInInventory.Num());

	// 장착 후 제거할 아이템 목록
	TArray<FItemData> ItemsToRemove;

	for (const FItemData& ShotgunPartItem : ShotgunPartsInInventory)
	{
		if (ShotgunPartItem.ItemClass)
		{
			// ItemClass에서 ShotgunPart 객체 생성
			ABShotgunPart* ShotgunPart = NewObject<ABShotgunPart>(this, ShotgunPartItem.ItemClass);

			if (ShotgunPart)
			{
				UE_LOG(LogTemp, Log, TEXT("Attempting to apply ShotgunPart: %s"), *ShotgunPart->PartName);

				// ShotgunPart를 샷건에 장착
				ShotgunPart->UseItem(this);

				// 제거할 아이템 목록에 추가
				ItemsToRemove.Add(ShotgunPartItem);

				UE_LOG(LogTemp, Log, TEXT("%s 파츠가 샷건에 장착되었습니다."), *ShotgunPart->PartName);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("샷건 파츠를 찾을 수 없습니다."));
			}
		}
	}

	// 🔹 루프 종료 후 아이템 삭제
	for (const FItemData& Item : ItemsToRemove)
	{
		BPlayerState->InventoryRemoveItem(Item);
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

void ABCharacter::UseItem(const FName& ItemName)
{
	State->UseItem(ItemName);
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
void ABCharacter::EquipGrenade()
{
	UE_LOG(LogTemp, Warning, TEXT("EquipGrenade() called!"));
	ActiveWeaponSlot = EWeaponSlot::Throwable;
	EquipWeaponByType(ActiveWeaponSlot);
}
void ABCharacter::UseFirstAidKit()
{
	// 플레이어 상태 확인
	ABPlayerState* BPlayerState = GetBPlayerState();
	if (!BPlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어 상태를 찾을 수 없습니다. 치료 아이템 사용 불가"));
		return;
	}

	// 🔹 인벤토리에서 구급상자 찾기
	FName HealingItemName = "FirstAidKit";  // 아이템 이름
	TArray<FItemData> HealingItems = BPlayerState->GetInventoryTypeItem(HealingItemName);

	if (HealingItems.Num() > 0)
	{
		// 첫 번째 구급상자 아이템을 사용
		ABBaseItem* HealingItem = HealingItems[0].ItemRef;
		if (HealingItem)
		{
			HealingItem->UseItem(this);  // 아이템 사용 (체력 회복)

			// ✅ 체력 회복 후 로그 출력
			UE_LOG(LogTemp, Log, TEXT("구급상자 사용 완료. 현재 체력: %f"),BPlayerState -> GetCurrentHealth());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("구급상자가 없습니다!"));
	}
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

float ABCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, FString::Printf(TEXT("TakeDamage ActualDamage %d"), ActualDamage));
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Magenta, FString::Printf(TEXT("TakeDamage DamageAmount %d"), DamageAmount));

	if (ABPlayerState* BPlayerState = GetPlayerState<ABPlayerState>())
	{
		BPlayerState->AddCurrentHealth(-DamageAmount);
		if (BPlayerState->GetCurrentHealth() <= 0)
		{
			BPlayerState->StartDeath();
		}
	}

	return ActualDamage;
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
		PlayerController->EquipGrenadeAction,
		ETriggerEvent::Completed,  // 🔹 키를 누르는 순간 실행
		this,
		&ABCharacter::EquipGrenade);
	EnhancedInput->BindAction(
		PlayerController->UseFristAidKitAction,
		ETriggerEvent::Completed,  // 🔹 키를 누르는 순간 실행
		this,
		&ABCharacter::UseFirstAidKit);
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
