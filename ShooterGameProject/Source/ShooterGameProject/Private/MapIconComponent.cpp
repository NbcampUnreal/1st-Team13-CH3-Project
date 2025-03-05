#include "MapIconComponent.h"
#include "BGameInstance.h"
#include "BUIManager.h"
#include "MapWidget.h"

UMapIconComponent::UMapIconComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMapIconComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		DelayTimerHandle,
		this,
		&UMapIconComponent::CreateIcon,
		0.1f,
		false
	);
}

void UMapIconComponent::CreateIcon()
{
	if (UBGameInstance* GameInstance = GetWorld()->GetGameInstance<UBGameInstance>())
	{
		if (UBUIManager* UIManager = GameInstance->GetUIManagerInstance())
		{
			if (UUserWidget* HUD = UIManager->GetHUDWidgetInstance())
			{
				if (UMapWidget* MapWidget = Cast<UMapWidget>(HUD->GetWidgetFromName(FName("MapWidget"))))
				{
					MapWidget->CreateIconForMinimap(GetOwner());
				}
			}
		}
	}
}

void UMapIconComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

