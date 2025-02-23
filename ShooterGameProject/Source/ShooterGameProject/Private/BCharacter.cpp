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

	// WeaponClass가 유효한지 확인
	if (WeaponClass)
	{
		UE_LOG(LogTemp, Log, TEXT("Weapon successfully attached to WeaponSocket!"));
		// 무기 스폰
		ABBaseWeapon* NewWeapon = GetWorld()->SpawnActor<ABBaseWeapon>(WeaponClass);

		if (NewWeapon)
		{
			// 캐릭터의 손 위치에 있는 소켓에 부착
			USkeletalMeshComponent* CharacterMesh = GetMesh();
			if (CharacterMesh)
			{
				FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
				// 무기가 부착된 후 로그 추가
				// 무기 장착 후 EquippedWeapons에 추가
				EquipWeapon(NewWeapon);
				UE_LOG(LogTemp, Log, TEXT("Weapon successfully attached to WeaponSocket!"));
				// 렌더링이 켜져 있는지 확인
				NewWeapon->SetActorEnableCollision(true);
				NewWeapon->SetActorHiddenInGame(false); // 총기 메시 보이게 하기
			}

			UE_LOG(LogTemp, Log, TEXT("무기 장착 완료!"));
		}
	}
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
		DraggingItem = Item;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("DraggingItem successfully set!"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("SetDraggingItem received NULL!"));
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

				// 디버그 메시지로 위치 확인
				GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow,
					FString::Printf(TEXT("Target Location: X=%.2f, Y=%.2f, Z=%.2f"),
						TargetLocation.X, TargetLocation.Y, TargetLocation.Z));

				// 아이템 위치 업데이트
				DraggingItem->SetActorEnableCollision(false);  // 충돌 비활성화
				DraggingItem->SetActorLocation(TargetLocation);
				DraggingItem->SetActorEnableCollision(true);   // 이동 후 충돌 다시 활성화
				bool bMoved = DraggingItem->SetActorLocation(TargetLocation);
				if (bMoved)
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("Dragging Item Moved!"));
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("SetActorLocation Failed!"));
				}
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
	// WeaponClass가 유효한지 확인
	if (WeaponClass)
	{
		// 무기 스폰
		ABBaseWeapon* NewWeapon = GetWorld()->SpawnActor<ABBaseWeapon>(WeaponClass);

		if (NewWeapon)
		{
			// 캐릭터의 손 위치에 있는 소켓에 부착
			USkeletalMeshComponent* CharacterMesh = GetMesh();
			if (CharacterMesh)
			{
				FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
				// 무기가 부착된 후 로그 추가
				// 무기 장착 후 EquippedWeapons에 추가
				EquipWeapon(NewWeapon);
				UE_LOG(LogTemp, Log, TEXT("Weapon successfully attached to WeaponSocket!"));
				// 렌더링이 켜져 있는지 확인
				NewWeapon->SetActorEnableCollision(true);
				NewWeapon->SetActorHiddenInGame(false); // 총기 메시 보이게 하기
			}

			UE_LOG(LogTemp, Log, TEXT("무기 장착 완료!"));
		}
	}
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
		UE_LOG(LogTemp, Log, TEXT("RifleFire"));

		// 🔹 타이머가 이미 실행 중이면 다시 설정하지 않음
		if (!GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ABCharacter::FireOnce, CurrentWeapon->FireRate, true);
		}
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
	if (NewWeapon)
	{
		// 1. 캐릭터의 손 위치에 있는 소켓에 무기 부착
		USkeletalMeshComponent* CharacterMesh = GetMesh();
		if (CharacterMesh)
		{
			// 🔹 무기 부착 (손 소켓에 장착)
			FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
			NewWeapon->AttachToComponent(GetMesh(), AttachRules, TEXT("WeaponSocket"));
			UE_LOG(LogTemp, Log, TEXT("WeaponType : %s"), *NewWeapon->WeaponType);  // ✅ 정상 동작
			if (NewWeapon->WeaponType == "Rifle") {
				// 🔹 상대 회전을 조정하여 총구가 앞쪽을 향하도록 설정
				FRotator AdjustedRotation(0.0f, -180.0f, 0.0f);  // Yaw 값(90도) 조정
				NewWeapon->SetActorRelativeRotation(AdjustedRotation);
			}
			if (NewWeapon->WeaponType == "Pistol") {
				// 🔹 상대 회전을 조정하여 총구가 앞쪽을 향하도록 설정
				FRotator AdjustedRotation(0.0f, 90.0f, 90.0f); // 예: Y축으로 90도 회전
				NewWeapon->SetActorRelativeRotation(AdjustedRotation);
			}
			// 🔹 무기에 캐릭터 정보 설정
			NewWeapon->SetOwnerCharacter(this);
			// 2. 무기 장착 후, 현재 활성화된 무기 슬롯에 무기 추가
			EquippedWeapons[(int32)EWeaponSlot::Primary] = NewWeapon;  // EWeaponSlot을 int32로 캐스팅하여 사용

			// 3. 무기 활성화 및 기타 설정 (옵션)
			NewWeapon->SetActorEnableCollision(true);  // 충돌 활성화
			NewWeapon->SetActorHiddenInGame(false);    // 게임에서 보이게 설정

			UE_LOG(LogTemp, Log, TEXT("Weapon successfully equipped and attached to WeaponSocket!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to equip weapon. NewWeapon is null."));
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