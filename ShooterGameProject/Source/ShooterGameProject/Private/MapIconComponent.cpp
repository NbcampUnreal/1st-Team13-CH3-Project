#include "MapIconComponent.h"
#include "Kismet/GameplayStatics.h"

UMapIconComponent::UMapIconComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMapIconComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UMapIconComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

