// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BCharacter.h"
#include "BAnimInstance.generated.h"

UENUM(BlueprintType)
enum class ECurrentMoveState : uint8
{
	Idle UMETA(DisplayName="Idle"),
	Walk UMETA(DisplayName = "Walk"),
	Jog UMETA(DisplayName = "Run")
};
UCLASS()
class SHOOTERGAMEPROJECT_API UBAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	class ABCharacter* OwnerPlayer;
	class ABPlayerState* PlayerState;
	class UBMovementComponent* PlayerMovement;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	FVector Velocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	FVector Velocity2D;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	FVector WorldLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	FRotator WorldRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	FVector Acceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	FVector Acceleration2D;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	float VelocityAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	float AccelerationAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	float GroundSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	bool IsAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	bool ShouldMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	bool IsFalling;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	bool IsFlying;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	bool IsCrouch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	ECurrentMoveState CurrentMoveState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	EWeaponSlot CurrentWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	bool IsJump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	bool IsCouch;
	float CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation) const;
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	float VelocityZ;
};
