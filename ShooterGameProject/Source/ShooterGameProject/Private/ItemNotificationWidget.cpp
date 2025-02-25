#include "ItemNotificationWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Blueprint/WidgetTree.h"

UItemNotificationWidget::UItemNotificationWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
	// TODO: i need a way to get a pointer to Textures and Materials (load once and refer to that instead of loading repeatedly)
	// below is for testing purposes
	ConstructorHelpers::FObjectFinder<UTexture2D> FirstAidKitIconFinder(
		TEXT("/Game/UI/Textures/T_FirstAidKit_Icon.T_FirstAidKit_Icon"));
	if (FirstAidKitIconFinder.Succeeded())
	{
		FirstAidKitIcon = FirstAidKitIconFinder.Object;
	}
	
	RobotoItalic = LoadObject<UObject>(this, TEXT("/Game/UI/Fonts/Font_RobotoItalic.Font_RobotoItalic"));

	ConstructorHelpers::FObjectFinder<UMaterialInstance> PurpleGlowMaterialFinder(
		TEXT("/Game/UI/Materials/MI_HUDGlowOutline_Purple.MI_HUDGlowOutline_Purple"));
	if (PurpleGlowMaterialFinder.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Material Succeeded"))
		PurpleGlowMaterial = PurpleGlowMaterialFinder.Object;
	}

	ImageSizeX = 84.f;
	ImageSizeY = 84.f;
}

void UItemNotificationWidget::DisplayNotification(FName ItemName)
{
	UTexture2D* ItemTexture = nullptr;
	FName DisplayItemName = "default";
	// TODO: Based on ItemName assign the corresponding texture and name
	if (ItemName == "FirstAidKit")
	{
		ItemTexture = FirstAidKitIcon;
		DisplayItemName = "First Aid Kit";
	}
	// else if (ItemName == "Grenade") 

	/********** Horizontal Box **********/
	UHorizontalBox* ItemNotificationHBox = Cast<UHorizontalBox>(GetWidgetFromName("ItemNotificationHBox"));
	if (ItemNotificationHBox == nullptr)
	{
		ItemNotificationHBox = WidgetTree->ConstructWidget<UHorizontalBox>(
			UHorizontalBox::StaticClass(), TEXT("ItemNotificationHBox"));
		if (ItemNotificationHBox) UE_LOG(LogTemp, Warning, TEXT("Item HBox Created"))
	}
	if (ItemNotificationHBox)
	{
		//ItemNotificationHBox->AddToRoot();

		/********** UImage **********/
		UImage* ItemNotificationImage = Cast<UImage>(GetWidgetFromName("ItemNotificationImage"));
		if (ItemNotificationImage == nullptr)
		{
			ItemNotificationImage = WidgetTree->ConstructWidget<UImage>(
				UImage::StaticClass(), TEXT("ItemNotificationImage"));
			if (ItemNotificationImage) UE_LOG(LogTemp, Warning, TEXT("Item Image Created"))
		}
		if (ItemNotificationImage)
		{
			ItemNotificationHBox->AddChildToHorizontalBox(ItemNotificationImage);

			// Set Brush and Size
			if (ItemTexture)
			{
				ItemNotificationImage->SetBrushFromTexture(ItemTexture);
				UE_LOG(LogTemp, Warning, TEXT("Item Texture Set"))
			}
			ItemNotificationImage->SetDesiredSizeOverride(FVector2D(ImageSizeX, ImageSizeY));

			// Set Alignment of UImage as a UHorizontalBoxSlot
			if (UHorizontalBoxSlot* ImageSlot = Cast<UHorizontalBoxSlot>(ItemNotificationImage->Slot))
			{
				ImageSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
				ImageSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
				UE_LOG(LogTemp, Warning, TEXT("Item Alignment Set"))
			}
		}

		/********** Vertical Box **********/
		UVerticalBox* ItemNotificationVBox = Cast<UVerticalBox>(GetWidgetFromName("ItemNotificationVBox"));
		if (ItemNotificationVBox == nullptr)
		{
			ItemNotificationVBox = WidgetTree->ConstructWidget<UVerticalBox>(
				UVerticalBox::StaticClass(), TEXT("ItemNotificationVBox"));

			if (ItemNotificationVBox) UE_LOG(LogTemp, Warning, TEXT("Item VBox Created"))
		}
		if (ItemNotificationVBox)
		{
			ItemNotificationHBox->AddChildToHorizontalBox(ItemNotificationVBox);

			// Set VerticalBox to Fill and center it vertically
			if (UHorizontalBoxSlot* VBoxSlot = Cast<UHorizontalBoxSlot>(ItemNotificationVBox->Slot))
			{
				VBoxSlot->SetSize(ESlateSizeRule::Fill);
				VBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
			}

			// Set "NEW ITEM ADDED" as the Item Notification title
			UTextBlock* ItemNotificationTitle = Cast<UTextBlock>(GetWidgetFromName("ItemNotificationTitle"));
			if (ItemNotificationTitle == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Item Title Created"))
					ItemNotificationTitle = WidgetTree->ConstructWidget<UTextBlock>(
						UTextBlock::StaticClass(), TEXT("ItemNotificationTitle"));
			}
			if (ItemNotificationTitle)
			{
				ItemNotificationVBox->AddChildToVerticalBox(ItemNotificationTitle);

				UE_LOG(LogTemp, Warning, TEXT("Item Title SetText"))
				ItemNotificationTitle->SetText(FText::FromString("NEW ITEM ADDED"));
				// TODO: Set Font, Size, outline material, etc.

				if (RobotoItalic)
				ItemNotificationTitle->SetFont(FSlateFontInfo(
					RobotoItalic, 24.f, NAME_None, FFontOutlineSettings(10.f, FLinearColor(1.f, 1.f, 1.f, 0.f))));
				ItemNotificationTitle->SetColorAndOpacity(FLinearColor(0.6f, 1.f, 0.f, 1.f));

				// Set Outline material
				if (PurpleGlowMaterial)
				{
					ItemNotificationTitle->SetFontOutlineMaterial(Cast<UMaterialInterface>(PurpleGlowMaterial));
				}

				// Set Padding as a Vertical Box Slot
				if (UVerticalBoxSlot* TitleSlot = Cast<UVerticalBoxSlot>(ItemNotificationTitle->Slot))
				{
					TitleSlot->SetPadding(FMargin(20.f, 0.f, 0.f, 0.f));
				}
			}

			UTextBlock* ItemNotificationName = Cast<UTextBlock>(GetWidgetFromName("ItemNotificationName"));
			if (ItemNotificationName == nullptr)
			{
				ItemNotificationName = WidgetTree->ConstructWidget<UTextBlock>(
					UTextBlock::StaticClass(), TEXT("ItemNotificationName"));
				UE_LOG(LogTemp, Warning, TEXT("Item Name Created"))
			}
			if (ItemNotificationName)
			{
				ItemNotificationVBox->AddChildToVerticalBox(ItemNotificationName);

				UE_LOG(LogTemp, Warning, TEXT("Item Name SetText"))
				ItemNotificationName->SetText(FText::FromName(DisplayItemName));
				ItemNotificationName->SetAutoWrapText(true);
				ItemNotificationName->SetColorAndOpacity(FLinearColor(0.6f, 1.f, 0.f, 1.f));
				// TODO: Set Font, Size, outline material, etc.
				if (RobotoItalic)
					ItemNotificationName->SetFont(FSlateFontInfo(
						RobotoItalic, 36.f, NAME_None, FFontOutlineSettings(10.f, FLinearColor(1.f, 1.f, 1.f, 0.f))));

				// Set Outline Material
				if (PurpleGlowMaterial)
					ItemNotificationName->SetFontOutlineMaterial(PurpleGlowMaterial);
			
				// Set Padding as a Vertical Box Slot
				if (UVerticalBoxSlot* NameSlot = Cast<UVerticalBoxSlot>(ItemNotificationName->Slot))
				{
					NameSlot->SetPadding(FMargin(75.f, 0.f, 0.f, 0.f));
				}
			}
		}
	}

	FadeInOutNotification();
}

void UItemNotificationWidget::RemoveNotification()
{
	if (!GetWorld() || GetWorld()->bIsTearingDown) return;

	if (UTextBlock* ItemNotificationTitle = Cast<UTextBlock>(GetWidgetFromName("ItemNotificationTitle")))
	{
		ItemNotificationTitle->RemoveFromParent();
		ItemNotificationTitle = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("NotificationTitle Removed"))
	}

	if (UTextBlock* ItemNotificationName = Cast<UTextBlock>(GetWidgetFromName("ItemNotificationName")))
	{
		ItemNotificationName->RemoveFromParent();
		ItemNotificationName = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("NotificationMessage Removed"))
	}

	if (UVerticalBox* ItemNotificationVBox = Cast<UVerticalBox>(GetWidgetFromName("ItemNotificationVBox")))
	{
		ItemNotificationVBox->RemoveFromParent();
		ItemNotificationVBox = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("ItemNotificationVBox Removed"))
	}

	if (UImage* ItemNotificationImage = Cast<UImage>(GetWidgetFromName("ItemNotificationImage")))
	{
		ItemNotificationImage->RemoveFromParent();
		ItemNotificationImage = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("ItemNotificationImage Removed"))
	}

	if (UHorizontalBox* ItemNotificationHBox = Cast<UHorizontalBox>(GetWidgetFromName("ItemNotificationHBox")))
	{
		ItemNotificationHBox->RemoveFromParent(); // No Parent. How should I remove this?
		//ItemNotificationHBox->RemoveFromRoot();
		ItemNotificationHBox = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("ItemNotificationHBox Removed"))
	}
}

