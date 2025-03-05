#pragma once

#include "CoreMinimal.h"
#include "Gameframework/Character.h"
#include "BPlayerController.h"
#include "BBaseWeapon.h"  // 旮半掣 氍搓赴 韥措灅鞀� 韽暔
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
	Pistol,    // 欤茧旮� (鞓�: 靻岇礉)
	Rifle,  // 氤挫“氍搓赴 (鞓�: 甓岇礉)
	ShotGun,  // 氤挫“氍搓赴 (鞓�: 甓岇礉)
	Melee,      // 攴检爲氍搓赴 (鞓�: 旃�, 霃勲伡)
	Throwable,   // 韴矙氍搓赴 (鞓�: 靾橂韮�)
	Max
};

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
	/** 旌愲Ν韯办潣 旃措⿺霛缄皜 氚旊澕氤措姅 氚╉枼鞚� 臧�鞝胳槫電� 頃垬 */
	FVector GetCameraForwardVector() const;
	// 馃敼 順勳灛 靷毄 欷戩澑 氍搓赴
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponSlot ActiveWeaponSlot;
	ABBaseWeapon* EquippedWeapon; // 현재 장착된 무기
	// 馃敼 順勳灛 鞛レ癌霅� 氍搓赴 (臧� 鞀’鞐� 頃措嫻頃橂姅 氍搓赴 鞝�鞛�)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	// 氚办棿 齑堦赴頇� 鞓堨嫓
	TArray<ABBaseWeapon*> EquippedWeapons;  // 鞀’鞐� 雽�鞚戫晿電� 氍搓赴 氚办棿

	void SetDraggingItem(AActor* NewItem);
	void PickupWeapon(ABBaseWeapon* NewWeapon);
	ABBaseWeapon* GetCurrentWeapon() const;
	void EquipPistol();
	void EquipRifle();
	void EquipShotgun();
	void EquipMelee();
	UFUNCTION(BlueprintCallable, Category = "Collect")
	TArray<class ABBaseItem*> GetNearItemArray() const;

	UFUNCTION(BlueprintCallable)
	void InventorySwitch();
	UFUNCTION(BlueprintCallable)
	void UseItem(const FName& ItemName);
protected:
	/** 카메라 줌 관련 변수 */
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float DefaultFOV = 90.0f;  // 기본 FOV 값

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ZoomedFOV = 60.0f;   // 줌 시 FOV 값

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ZoomInterpSpeed = 10.0f;  // FOV 변화 속도

	bool bIsAiming = false;  // 줌 상태 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCameraComponent> CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USpringArmComponent> SpringArm;
	TObjectPtr<class UCapsuleComponent> Collision;
	TObjectPtr<class USkeletalMeshComponent> Skeletal;
	TObjectPtr<class UBMovementComponent> MoveComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USphereComponent> CollectNearItem;
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
	UFUNCTION()
	void AimStart(const FInputActionValue& Value);
	UFUNCTION()
	void AimStop(const FInputActionValue& Value);
	
	UFUNCTION()
	void ZoomStart(const FInputActionValue& Value);
	UFUNCTION()
	void ZoomStop(const FInputActionValue& Value);
	
	UFUNCTION()
	void StartDragging(const FInputActionValue& Value);
	UFUNCTION()
	void StopDragging(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void ShowInventory();
	UFUNCTION(BlueprintCallable)
	void CloseInventory();
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void StopFire();
	
	void EquipWeaponByType(EWeaponSlot Slot);
	FTimerHandle FireTimerHandle;

	
private:
	UPROPERTY()
	FReplicatedAcceleration ReplicatedAcceleration;

	FTimerHandle DragUpdateTimer; // 霌滊灅攴� 韮�鞚措ǜ
	/** 줌 상태 변경 시만 보간을 실행하기 위한 타이머 */
	FTimerHandle ZoomTimerHandle;
	bool bIsDragging = false;
	ABBaseItem* DraggingItem = nullptr;

	void UpdateDragging(); // 霌滊灅攴� 鞙勳箻 鞐呺嵃鞚错姼
	class ABPlayerState* State;
};