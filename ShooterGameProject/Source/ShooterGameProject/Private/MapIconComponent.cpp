#include "MapIconComponent.h"
#include "BGameInstance.h"
#include "BUIManager.h"
#include "MapWidget.h"

UMapIconComponent::UMapIconComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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

void UMapIconComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	RemoveIcon();
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

void UMapIconComponent::RemoveIcon()
{
	if (UBGameInstance* GameInstance = GetWorld()->GetGameInstance<UBGameInstance>())
	{
		if (UBUIManager* UIManager = GameInstance->GetUIManagerInstance())
		{
			if (UUserWidget* HUD = UIManager->GetHUDWidgetInstance())
			{
				if (UMapWidget* MapWidget = Cast<UMapWidget>(HUD->GetWidgetFromName(FName("MapWidget"))))
				{
					MapWidget->RemoveIconFromMinimap(GetOwner());
				}
			}
		}
	}
}