#include "MapWidget.h"
#include "Components/Image.h"
#include "BCharacter.h"

void UMapWidget::UpdateMap()
{
	// Map
	if (UImage* MapImage = Cast<UImage>(GetWidgetFromName(TEXT("MapImage"))))
	{
		// TODO: Update this image... to what?..
	}

	// Player Direction
	if (UImage* PlayerMarker = Cast<UImage>(GetWidgetFromName(TEXT("PlayerMarker"))))
	{
		// TODO: Change Image to something better
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (ABCharacter* BCharacter = Cast<ABCharacter>(PlayerController->GetCharacter()))
			{
				float CharacterYaw = static_cast<float>(BCharacter->GetActorRotation().Yaw);
				PlayerMarker->SetRenderTransformAngle(CharacterYaw);
			}
		}
	}
}
