#include "MapWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
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

