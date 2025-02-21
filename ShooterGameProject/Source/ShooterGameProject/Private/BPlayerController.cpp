#include "BPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

ABPlayerController::ABPlayerController() :
	InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr), 
	AttackAction(nullptr),
	ReloadAction(nullptr)
{

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
				Subsystem->AddMappingContext(InputMappingContext, 0); // 우선순위 0
			}
		}
	}
}
