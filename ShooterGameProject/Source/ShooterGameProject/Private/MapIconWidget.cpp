#include "MapIconWidget.h"
#include "MapIconComponent.h"
#include "MapWidget.h"
#include "BCharacter.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Blueprint/WidgetTree.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

void UMapIconWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(IconOwner))
	{
		if (UMapIconComponent* MapIconComponent = IconOwner->GetComponentByClass<UMapIconComponent>())
		{
			MapIconImage->SetBrushFromTexture(MapIconComponent->IconTexture);
		}
	}

	MinimapWidget = nullptr;
	if (UOverlay* MinimapOverlay = Cast<UOverlay>(GetParent()))
	{
		if (UWidgetTree* MinimapTree = Cast<UWidgetTree>(MinimapOverlay->GetOuter()))
		{
			MinimapWidget = Cast<UMapWidget>(MinimapTree->GetOuter());

		}
	}
	if (MinimapWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("MapIconWidget Failed to get a reference to MapWidget"))
	}
}

void UMapIconWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsValid(MinimapWidget) && IsValid(IconOwner))
	{
		if (UMaterialParameterCollectionInstance* MPCInstance =
			GetWorld()->GetParameterCollectionInstance(MinimapWidget->MPCMinimap))
		{
			float ZoomValue, SizeValue, PlayerXPos, PlayerYPos;
			MPCInstance->GetScalarParameterValue(FName("Zoom"), ZoomValue);
			MPCInstance->GetScalarParameterValue(FName("Size"), SizeValue);
			MPCInstance->GetScalarParameterValue(FName("XPos"), PlayerXPos);
			MPCInstance->GetScalarParameterValue(FName("YPos"), PlayerYPos);
			float Scale = (SizeValue / MinimapWidget->MinimapSize) * ZoomValue;
			float IconX  = PlayerXPos - MinimapWidget->CaptureOffsetX - IconOwner->GetActorLocation().X;
			float IconY = -1 * (PlayerYPos - MinimapWidget->CaptureOffsetY - IconOwner->GetActorLocation().Y);
			IconX /= Scale;
			IconY /= Scale;
			DetermineLocationOnMinimap(FVector2D(0.f, 0.f), FVector2D(IconX, IconY));
		}
	}
}

void UMapIconWidget::DetermineLocationOnMinimap(FVector2D CharacterLoc, FVector2D IconLoc)
{
	double Angle = atan2(CharacterLoc.Y - IconLoc.Y, CharacterLoc.X - IconLoc.X);
	float Distance = FMath::Clamp(IconLoc.Length(), 0.f, MinimapWidget->MinimapSize * 0.5);
	SetRenderTranslation(FVector2D(sin(Angle) * Distance * -1, cos(Angle) * Distance * -1));
}
