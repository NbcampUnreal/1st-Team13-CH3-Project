#include "MapWidget.h"
#include "MapIconWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "BCharacter.h"

UMapWidget::UMapWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> MPCFinder(TEXT("/Game/UI/Minimap/MPC_Minimap.MPC_Minimap"));
	if (MPCFinder.Succeeded())
	{
		MPCMinimap = MPCFinder.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MapWidget failed to load MPC"));
	}

	// Scene capture component offset
	CaptureOffsetX = 280.f;
	CaptureOffsetY = 2403.f;
	MinimapSize = 250.f;
}

void UMapWidget::UpdateMap()
{
	if (GetWorld() && !GetWorld()->bIsTearingDown)
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (ABCharacter* BCharacter = Cast<ABCharacter>(PlayerController->GetCharacter()))
			{
				if (UMaterialParameterCollectionInstance* MPCInstance = GetWorld()->GetParameterCollectionInstance(MPCMinimap))
				{
					if (MapImage)
					{
						float XPos = BCharacter->GetActorLocation().X + CaptureOffsetX;
						float YPos = BCharacter->GetActorLocation().Y + CaptureOffsetY;
						MPCInstance->SetScalarParameterValue(FName("XPos"), XPos);
						MPCInstance->SetScalarParameterValue(FName("YPos"), YPos);
					}

					if (PlayerMarker)
					{
						float CharacterYaw = static_cast<float>(BCharacter->GetActorRotation().Yaw);
						PlayerMarker->SetRenderTransformAngle(CharacterYaw);
					}
				}
			}
		}
	}
}

void UMapWidget::CreateIconForMinimap(AActor* OwningActor)
{
	if (MapIconWidgetClass)
	{
		if (UMapIconWidget* MapIconWidget = CreateWidget<UMapIconWidget>(this, MapIconWidgetClass))
		{
			MapIconWidget->IconOwner = OwningActor;
			
			MapOverlay->AddChildToOverlay(MapIconWidget);
			if (UOverlaySlot* MapIconSlot = Cast<UOverlaySlot>(MapIconWidget->Slot))
			{
				MapIconSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
				MapIconSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
			}

			MapIconArray.Add(MapIconWidget);
		}
	}
}

void UMapWidget::RemoveIconFromMinimap(AActor* OwningActor)
{
	for (UMapIconWidget* MapIconWidget : MapIconArray)
	{
		if (MapIconWidget->IconOwner == OwningActor)
		{
			MapIconWidget->RemoveFromParent();
			MapIconArray.Remove(MapIconWidget);
			return;
		}
	}
}

