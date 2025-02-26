#include "BPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

ABPlayerController::ABPlayerController()
{
	// 마우스 입력 활성화
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ABPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (WidgetHUDClass)
	{
		WidgetHUDInstance = CreateWidget<UUserWidget>(this, WidgetHUDClass);
	}
	if (WidgetHUDInstance != nullptr)
	{
		WidgetHUDInstance->AddToViewport();
		SetInputMode(FInputModeGameOnly());
	}

	if (auto LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0); // �켱���� 0
			}
		}
	}
}

