#pragma once

#include "CoreMinimal.h"
#include "Gameframework/Character.h"
#include "BPlayerController.h"
#include "BPlayerState.h"
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
	Primary,    // 欤茧旮� (鞓�: 靻岇礉)
	Secondary,  // 氤挫“氍搓赴 (鞓�: 甓岇礉)
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

	// 馃敼 順勳灛 鞛レ癌霅� 氍搓赴 (臧� 鞀’鞐� 頃措嫻頃橂姅 氍搓赴 鞝�鞛�)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	// 氚办棿 齑堦赴頇� 鞓堨嫓
	TArray<ABBaseWeapon*> EquippedWeapons;  // 鞀’鞐� 雽�鞚戫晿電� 氍搓赴 氚办棿

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
	UFUNCTION()
	void AimStart(const FInputActionValue& Value);
	UFUNCTION()
	void StartDragging(const FInputActionValue& Value);
	UFUNCTION()
	void StopDragging(const FInputActionValue& Value);
	UFUNCTION()
	void AimStop(const FInputActionValue& Value);
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void FireOnce();
	void StopFire();
	FTimerHandle FireTimerHandle;
private:
	UPROPERTY()
	FReplicatedAcceleration ReplicatedAcceleration;

	FTimerHandle DragUpdateTimer; // 霌滊灅攴� 韮�鞚措ǜ
	bool bIsDragging = false;
	ABBaseItem* DraggingItem = nullptr;

	void UpdateDragging(); // 霌滊灅攴� 鞙勳箻 鞐呺嵃鞚错姼
};