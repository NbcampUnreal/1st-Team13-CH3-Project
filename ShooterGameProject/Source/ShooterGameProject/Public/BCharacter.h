#pragma once

#include "CoreMinimal.h"
#include "Gameframework/Character.h"
#include "BPlayerController.h"
#include "BBaseWeapon.h"  // 旮半掣 氍搓赴 韥措灅鞀� 韽暔
#include "BCharacter.generated.h"
struct FInputActionValue;

UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	Unarmed UMETA(DisplayName = "Unarmed"),
	Pistol UMETA(DisplayName = "Pistol"),    // 欤茧旮� (鞓�: 靻岇礉)
	Rifle UMETA(DisplayName = "Rifle"),  // 氤挫“氍搓赴 (鞓�: 甓岇礉)
	ShotGun UMETA(DisplayName = "ShotGun"),  // 氤挫“氍搓赴 (鞓�: 甓岇礉)
	Melee UMETA(DisplayName = "Melee"),      // 攴检爲氍搓赴 (鞓�: 旃�, 霃勲伡)
	Throwable UMETA(DisplayName = "Throwable"),   // 韴矙氍搓赴 (鞓�: 靾橂韮�)
	Max
};

UCLASS()
class ABCharacter :
	public ACharacter
{
	GENERATED_BODY()
public:
	ABCharacter(const FObjectInitializer& ObjectInitializer);
	//void PlayAnimation(UAnimMontage* Montage);
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

	void PickupWeapon(ABBaseWeapon* NewWeapon);
	ABBaseWeapon* GetCurrentWeapon() const;
	void EquipPistol();
	void EquipRifle();
	void EquipShotgun();
	void EquipMelee();
	void EquipGrenade();
	void UseFirstAidKit();
	UFUNCTION(BlueprintCallable, Category = "Collect")
	TArray<class ABBaseItem*> GetNearItemArray() const;

	UFUNCTION(BlueprintCallable)
	void InventorySwitch();
	UFUNCTION(BlueprintCallable)
	void UseItem(const FName& ItemName);
	int32 GrenadeCount = 0;
	UFUNCTION(BlueprintCallable)
	void Reload(const struct FInputActionValue& Value);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ReloadAnimation;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsWeaponFire;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsReload;
	//UFUNCTION(NetMulticast, unreliable)
	//void FastSharedReplication(const FSharedRepMovement& SharedRepMovement);
	UFUNCTION(BlueprintCallable)
	void Move(const struct FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void Look(const struct FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void StartJump(const struct FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void StopJump(const struct FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void StartSprint(const struct FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void StopSprint(const struct FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void Attack(const struct FInputActionValue& Value);
	void UnequipGrenade();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Health")
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	


	UFUNCTION()
	void ZoomStart(const FInputActionValue& Value);
	UFUNCTION()
	void ZoomStop(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void ShowInventory();
	UFUNCTION(BlueprintCallable)
	void CloseInventory();
	virtual void BeginPlay() override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void StopFire();	
	void EquipWeaponByType(EWeaponSlot Slot);
	void EquipRifleParts();
	void EquipPistolParts();
	void EquipShotgunParts();
	FTimerHandle FireTimerHandle;

public:
	UFUNCTION(BlueprintCallable)
	void OnReloadMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION(BlueprintCallable)
	bool GetIsWeaponFire() const;
	UFUNCTION(BlueprintCallable)
	bool IsReload() const;
	// 애니메이션 노티파이 혹은 몽타주를 통해 넣어야한다.
	UFUNCTION(BlueprintCallable)
	void RelaoadCompleted();
	UFUNCTION(BlueprintCallable)
	void AttackCompleted();
private:
	FTimerHandle ZoomTimerHandle;
	class ABPlayerState* State;
	void OnReloadMontageEnd(UAnimMontage* Montage, bool bInterrupted);
};