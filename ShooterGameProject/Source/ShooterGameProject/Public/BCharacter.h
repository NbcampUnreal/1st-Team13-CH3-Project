#pragma once

#include "CoreMinimal.h"
<<<<<<< HEAD
#include "Gameframework/Character.h"
=======
#include "Gameframework/character.h"
>>>>>>> 9c09620f6cf655473203ff14d7a7d9ac254ea373
#include "BPlayerController.h"
#include "BPlayerState.h"
#include "BCharacter.generated.h"

USTRUCT()
<<<<<<< HEAD
struct FReplicatedAcceleration
=======
struct FLyraReplicatedAcceleration
>>>>>>> 9c09620f6cf655473203ff14d7a7d9ac254ea373
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 AccelXYRadians = 0;	// Direction of XY accel component, quantized to represent [0, 2*pi]

	UPROPERTY()
	uint8 AccelXYMagnitude = 0;	//Accel rate of XY component, quantized to represent [0, MaxAcceleration]

	UPROPERTY()
	int8 AccelZ = 0;	// Raw Z accel rate component, quantized to represent [-MaxAcceleration, MaxAcceleration]
};

<<<<<<< HEAD
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
class SHOOTERGAMEPROJECT_API ABCharacter :
=======
/** The type we use to send FastShared movement updates. */
USTRUCT()
struct FSharedRepMovement
{
	GENERATED_BODY()

	FSharedRepMovement();

	bool FillForCharacter(ACharacter* Character);
	bool Equals(const FSharedRepMovement& Other, ACharacter* Character) const;

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	UPROPERTY(Transient)
	FRepMovement RepMovement;

	UPROPERTY(Transient)
	float RepTimeStamp = 0.0f;

	UPROPERTY(Transient)
	uint8 RepMovementMode = 0;

	UPROPERTY(Transient)
	bool bProxyIsJumpForceApplied = false;

	UPROPERTY(Transient)
	bool bIsCrouched = false;
};
UCLASS()
class ABCharacter : 
>>>>>>> 9c09620f6cf655473203ff14d7a7d9ac254ea373
	public ACharacter
{
	GENERATED_BODY()
public:
<<<<<<< HEAD
	ABCharacter(const FObjectInitializer& ObjectInitializer);
	UFUNCTION(BlueprintCallable, Category = "Character")
	class ABPlayerState* GetBPlayerState() const;
	UFUNCTION(BlueprintCallable, Category = "Character")
	class ABPlayerController* GetBPlayerController() const;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCameraComponent> CameraComp;
=======
	ABCharacter();
	UFUNCTION(BlueprintCallable, Category = "Character")
	ABPlayerController* GetLyraPlayerController() const;
	UFUNCTION(BlueprintCallable, Category = "Character")
	ABPlayerState* GetLyraPlayerState() const;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCameraComponent> CameraComponent;
>>>>>>> 9c09620f6cf655473203ff14d7a7d9ac254ea373
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USpringArmComponent> SpringArm;
	TObjectPtr<class UCapsuleComponent> Collision;
	TObjectPtr<class USkeletalMeshComponent> Skeletal;
<<<<<<< HEAD
	TObjectPtr<class UBMovementComponent> MoveComp;
protected:
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
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UPROPERTY()
	FReplicatedAcceleration ReplicatedAcceleration;
=======
	TObjectPtr<UCharacterMovementComponent> MoveCompoment;
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	int32 MaxHealth;
	int32 CurrentHealth;
	int32 AttackDamage;
	int32 Level;
	int32 MaxExperience;
	int32 CurrentExperience;
public:
	void OnDeath();
	void AddCurrentHelth(int32 Helth);
	void AddExp(int32 Exp);
	void Attack(AActor* Actor);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
private:
	void LevelUP();
	//	케릭터 레벨업을 위한 
	//	Instance에 있는게 좋은거 같다.
	TMap<int32, int32> LevelTable;
>>>>>>> 9c09620f6cf655473203ff14d7a7d9ac254ea373
};