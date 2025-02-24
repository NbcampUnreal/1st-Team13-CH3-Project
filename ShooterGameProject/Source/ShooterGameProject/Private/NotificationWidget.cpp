#include "NotificationWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Blueprint/WidgetTree.h"

void UNotificationWidget::DisplayNotification(FString Title, FString Message)
{
	UVerticalBox* NotificationVBox = Cast<UVerticalBox>(GetWidgetFromName("NotificationVBox"));
	if (NotificationVBox == nullptr)
	{
		NotificationVBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("NotificationVBox"));
		NotificationVBox->AddToRoot();
	}
	if (NotificationVBox)
	{
		UTextBlock* NotificationTitleText = Cast<UTextBlock>(GetWidgetFromName("NotificationTitleText"));
		if (NotificationTitleText == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("NotificationTitleText Created"))
			NotificationTitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("NotificationTitleText"));
			NotificationVBox->AddChildToVerticalBox(NotificationTitleText);
		}
		if (NotificationTitleText)
		{
			UE_LOG(LogTemp, Warning, TEXT("NotificationTitle SetText"))
			NotificationTitleText->SetText(FText::FromString(Title));
			NotificationTitleText->SetAutoWrapText(true);
		}

		UTextBlock* NotificationMessageText = Cast<UTextBlock>(GetWidgetFromName("NotificationMessageText"));
		if (NotificationMessageText == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("NotificationMessageText Created"))
			NotificationMessageText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("NotificationMessageText"));
			NotificationVBox->AddChildToVerticalBox(NotificationMessageText);
		}
		if (NotificationMessageText)
		{
			UE_LOG(LogTemp, Warning, TEXT("NotificationMessage SetText"))
			NotificationMessageText->SetText(FText::FromString(Message));
			NotificationMessageText->SetAutoWrapText(true);
		}
	}

	FadeInOutNotification();
}

void UNotificationWidget::RemoveNotification()
{
	if (UTextBlock* NotificationTitleText = Cast<UTextBlock>(GetWidgetFromName("NotificationTitleText")))
	{
		NotificationTitleText->RemoveFromParent();
		NotificationTitleText = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("NotificationTitle Removed"))
	}

	if (UTextBlock* NotificationMessageText = Cast<UTextBlock>(GetWidgetFromName("NotificationMessageText")))
	{
		NotificationMessageText->RemoveFromParent();
		NotificationMessageText = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("NotificationMessage Removed"))
	}

	if (UVerticalBox* NotificationVBox = Cast<UVerticalBox>(GetWidgetFromName("NotificationVBox")))
	{
		NotificationVBox->RemoveFromParent();
		NotificationVBox = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("NotificationVBox Removed"))
	}
}
