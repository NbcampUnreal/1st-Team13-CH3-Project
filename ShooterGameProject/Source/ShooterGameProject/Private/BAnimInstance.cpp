#include "BAnimInstance.h"
#include "BCharacter.h"
#include "BPlayerState.h"
#include "BGameInstance.h"
#include "BPlayerController.h"
#include "BMovementComponent.h"

float UBAnimInstance::CalculateDirection(const FVector& RVelocity, const FRotator& BaseRotation) const
{
	if (!RVelocity.IsNearlyZero())
	{
		FMatrix RotMatrix = FRotationMatrix(BaseRotation);
		FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		FVector NormalizedVel = RVelocity.GetSafeNormal2D();

		// get a cos(alpha) of forward vector vs velocity
		float ForwardCosAngle = FVector::DotProduct(ForwardVector, NormalizedVel);
		// now get the alpha and convert to degree
		float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

		// depending on where right vector is, flip it
		float RightCosAngle = FVector::DotProduct(RightVector, NormalizedVel);
		if (RightCosAngle < 0)
		{
			ForwardDeltaDegree *= -1;
		}

		return ForwardDeltaDegree;
	}

	return 0.f;
}
//#include "KismetAnimationLibrary.h"
void UBAnimInstance::NativeInitializeAnimation()
{
	OwnerPlayer = Cast<ABCharacter>(TryGetPawnOwner());
	if (OwnerPlayer)
	{
		PlayerMovement = Cast<UBMovementComponent>(OwnerPlayer->GetCharacterMovement());
		PlayerState = Cast<ABPlayerState>(OwnerPlayer->GetPlayerState());
	}
}

void UBAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (OwnerPlayer == nullptr || PlayerMovement == nullptr)
		return;

	IsJump = false;
	WorldLocation = OwnerPlayer->GetActorLocation();
	Velocity = OwnerPlayer->GetVelocity();
	Velocity2D = Velocity * FVector(1.f, 1.f, 0.f);
	VelocityZ = Velocity.Z;
	GroundSpeed = Velocity2D.Length();
	Acceleration = OwnerPlayer->GetCharacterMovement()->GetCurrentAcceleration();
	Acceleration2D = Acceleration * FVector(1.f, 1.f, 0.f);
	WorldRotation = OwnerPlayer->GetActorRotation();
	VelocityAngle = CalculateDirection(Velocity, WorldRotation);
	AccelerationAngle = CalculateDirection(Acceleration, WorldRotation);
	IsAcceleration = !FMath::IsNearlyEqual(Acceleration2D.Length(), 0, 0.001);
	IsCrouch = OwnerPlayer->bIsCrouched;
	IsFalling = PlayerMovement->IsFalling();
	IsFlying = PlayerMovement->IsFlying();
	
	if (GroundSpeed <= 3.f && Velocity2D.IsNearlyZero())
	{
		ShouldMove = true;
		CurrentMoveState = ECurrentMoveState::Idle;
	}
	else
	{
		ShouldMove = false;
	}
	
	if (ShouldMove == false)
	{
		if (GroundSpeed >= 400.f)
		{
			CurrentMoveState = ECurrentMoveState::Jog;
		}
		else
		{
			CurrentMoveState = ECurrentMoveState::Walk;
		}
	}


	if (!FMath::IsNearlyZero(VelocityZ) && IsFalling)
	{
		IsJump = true;
	}
	
	CurrentWeapon = OwnerPlayer->ActiveWeaponSlot;
}
