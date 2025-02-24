#pragma once

#include "CoreMinimal.h"
#include "Gameframework/Character.h"
#include "BPlayerController.h"
#include "BPlayerState.h"
#include "BBaseWeapon.h"  // 기본 무기 클래스 포함
#include "BCharacter.generated.h"
struct FInputActionValue;

USTRUCT()
struct FReplicatedAcceleration
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 AccelXYRadians = 0;	// Direction of XY accel component, quantized to represent [0, 2*pi]

	UPROPERTY()
	uint8 AccelXYMagnitude = 0;	//Accel rate of XY component, quantized to represent [0, MaxAcceleration]

	UPROPERTY()
	int8 AccelZ = 0;	// Raw Z accel rate component, quantized to represent [-MaxAcceleration, MaxAcceleration]
};
UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	Primary,    // 주무기 (예: 소총)
	Secondary,  // 보조무기 (예: 권총)
	Melee,      // 근접무기 (예: 칼, 도끼)
	Throwable,   // 투척무기 (예: 수류탄)
	Max
};
//USTRUCT()
//struct FSharedRepMovement
//{
//	GENERATED_BODY()
//
//	FSharedRepMovement();
//
//	bool FillForCharacter(ACharacter* Character);
//	bool Equals(const FSharedRepMovement& Other, ACharacter* Character) const;
//
//	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
//
//	UPROPERTY(Transient)
//	FRepMovement RepMovement;
//
//	UPROPERTY(Transient)
//	float RepTimeStamp = 0.0f;
//
//	UPROPERTY(Transient)
//	uint8 RepMovementMode = 0;
//
//	UPROPERTY(Transient)
//	bool bProxyIsJumpForceApplied = false;
//
//	UPROPERTY(Transient)
//	bool bIsCrouched = false;
//};

//template<>
//struct TStructOpsTypeTraits<FSharedRepMovement> : public TStructOpsTypeTraitsBase2<FSharedRepMovement>
//{
//	enum
//	{
//		WithNetSerializer = true,
//		WithNetSharedSerialization = true,
//	};
//};

UCLASS()
class ABCharacter :
	public ACharacter
{
	GENERATED_BODY()
public:
	ABCharacter(const FObjectInitializer& ObjectInitializer);
	UFUNCTION(BlueprintCallable, Category = "Character")
	class ABPlayerState* GetBPlayerState() const;
	UFUNCTION(BlueprintCallable, Category = "Character")
	class ABPlayerController* GetBPlayerController() const;
	/** 캐릭터의 카메라가 바라보는 방향을 가져오는 함수 */
	FVector GetCameraForwardVector() const;
	// 🔹 현재 사용 중인 무기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponSlot ActiveWeaponSlot;

	// 🔹 현재 장착된 무기 (각 슬롯에 해당하는 무기 저장)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	// 배열 초기화 예시
	TArray<ABBaseWeapon*> EquippedWeapons;  // 슬롯에 대응하는 무기 배열

	void SetDraggingItem(AActor* NewItem);
	void EquipWeapon(ABBaseWeapon* NewWeapon);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCameraComponent> CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USpringArmComponent> SpringArm;
	TObjectPtr<class UCapsuleComponent> Collision;
	TObjectPtr<class USkeletalMeshComponent> Skeletal;
	TObjectPtr<class UBMovementComponent> MoveComp;
	//UFUNCTION(NetMulticast, unreliable)
	//void FastSharedReplication(const FSharedRepMovement& SharedRepMovement);
	UFUNCTION()
	void Move(const struct FInputActionValue& Value);
	UFUNCTION()
	void Look(const struct FInputActionValue& Value);
	UFUNCTION()
	void StartJump(const struct FInputActionValue& Value);
	UFUNCTION()
	void StopJump(const struct FInputActionValue& Value);
	UFUNCTION()
	void StartSprint(const struct FInputActionValue& Value);
	UFUNCTION()
	void StopSprint(const struct FInputActionValue& Value);
	UFUNCTION()
	void Attack(const struct FInputActionValue& Value);
	UFUNCTION()
	void Reload(const struct FInputActionValue& Value);
	void StartDragging();
	void StopDragging();
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void FireOnce();
	void StopFire();
	FTimerHandle FireTimerHandle;
private:
	UPROPERTY()
	FReplicatedAcceleration ReplicatedAcceleration;

	FTimerHandle DragUpdateTimer; // 드래그 타이머
	bool bIsDragging = false;
	ABBaseItem* DraggingItem = nullptr;

	void UpdateDragging(); // 드래그 위치 업데이트
};