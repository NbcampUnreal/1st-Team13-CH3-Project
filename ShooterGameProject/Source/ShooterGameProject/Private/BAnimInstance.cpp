#include "BAnimInstance.h"
#include "BCharacter.h"
#include "BPlayerState.h"
#include "BGameInstance.h"
#include "BPlayerController.h"
#include "BMovementComponent.h"
void UBAnimInstance::NativeInitializeAnimation()
{
	OwnerPlayer = Cast<ABCharacter>(TryGetPawnOwner());
	PlayerState = Cast<ABPlayerState>(OwnerPlayer->GetBPlayerState());
	PlayerMovement = Cast<UBMovementComponent>(OwnerPlayer->GetCharacterMovement());
}

void UBAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	WorldLocation = OwnerPlayer->GetActorLocation();
	Velocity = OwnerPlayer->GetVelocity();
	Velocity2D = Velocity * FVector(1.f, 1.f, 0.f);
	GroundSpeed = Velocity2D.Length();
	Acceleration = OwnerPlayer->GetCharacterMovement()->GetCurrentAcceleration();
	Acceleration2D = Acceleration * FVector(1.f, 1.f, 0.f);
	WorldRotation = OwnerPlayer->GetActorRotation();
	VelocityAngle = CalculateDirection(Velocity, WorldRotation);
	AccelerationAngle = CalculateDirection(Acceleration, WorldRotation);
	if (PlayerMovement->MaxWalkSpeed <= 200.f)
	{
		CurrentMoveState = ECurrentMoveState::Walk;
	}
	else if (PlayerMovement->MaxWalkSpeed > 200.f)
	{
		CurrentMoveState = ECurrentMoveState::Jog;
	}
	if (GroundSpeed <= 3.f && !Velocity2D.IsNearlyZero())
	{
		ShouldMove = true;
		CurrentMoveState = ECurrentMoveState::Idle;
	}
	
	IsAcceleration = !FMath::IsNearlyEqual(Acceleration2D.Length(), 0, 0.001);
	IsCrouch = OwnerPlayer->bIsCrouched;
	IsFalling = PlayerMovement->IsFalling();
	IsFlying = PlayerMovement->IsFlying();
}
