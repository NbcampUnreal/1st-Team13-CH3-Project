// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BAnimInstance.generated.h"

UENUM(BlueprintType)
enum class ECurrentWeapon : uint8
{
	Unarmed UMETA(DisplayName = "ºñ¹«Àå"),
	Pistol UMETA(DisplayName = "±ÇÃÑ"),
	Rifle UMETA(DisplayName = "±â°üÃÑ"),
	Shotgun UMETA(DisplayName = "±â°üÃÑ")	
};
UENUM(BlueprintType)
enum class ECurrentMoveState : uint8
{
	Idle UMETA(DisplayName="±âº»"),
	Walk UMETA(DisplayName = "°È±â"),
	Jog UMETA(DisplayName = "¶Ù±â")
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	FVector Velocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	FVector Velocity2D;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	FVector WorldLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	FRotator WorldRotation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	FVector Acceleration;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	FVector Acceleration2D;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	float VelocityAngle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	float AccelerationAngle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	float GroundSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	bool IsAcceleration;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	bool ShouldMove;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	bool IsFalling;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	bool IsFlying;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	bool IsCrouch;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	ECurrentWeapon CurrentWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CurrentStateValue")
	ECurrentMoveState CurrentMoveState;
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
