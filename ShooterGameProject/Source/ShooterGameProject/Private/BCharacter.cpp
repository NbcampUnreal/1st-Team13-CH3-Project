#include "BCharacter.h"

ABCharacter::ABCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

