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
		NotificationVBox = WidgetTree->ConstructWidget<UVerticalBox>(
			UVerticalBox::StaticClass(), TEXT("NotificationVBox"));
	}
	if (NotificationVBox)
	{
		UTextBlock* NotificationTitleText = Cast<UTextBlock>(GetWidgetFromName("NotificationTitleText"));
		if (NotificationTitleText == nullptr)
		{
			NotificationTitleText = WidgetTree->ConstructWidget<UTextBlock>(
				UTextBlock::StaticClass(), TEXT("NotificationTitleText"));
		}
		if (NotificationTitleText)
		{
			NotificationVBox->AddChildToVerticalBox(NotificationTitleText);
			NotificationTitleText->SetText(FText::FromString(Title));
			NotificationTitleText->SetAutoWrapText(true);
		}

		UTextBlock* NotificationMessageText = Cast<UTextBlock>(GetWidgetFromName("NotificationMessageText"));
		if (NotificationMessageText == nullptr)
		{
			NotificationMessageText = WidgetTree->ConstructWidget<UTextBlock>(
				UTextBlock::StaticClass(), TEXT("NotificationMessageText"));
		}
		if (NotificationMessageText)
		{
			NotificationVBox->AddChildToVerticalBox(NotificationMessageText);
			NotificationMessageText->SetText(FText::FromString(Message));
			NotificationMessageText->SetAutoWrapText(true);
		}
	}

	FadeInOutNotification();
}

void UNotificationWidget::RemoveNotification()
{
	if (!GetWorld() || GetWorld()->bIsTearingDown) return;

	if (UTextBlock* NotificationTitleText = Cast<UTextBlock>(GetWidgetFromName("NotificationTitleText")))
	{
		NotificationTitleText->RemoveFromParent();
		NotificationTitleText = nullptr;
	}

	if (UTextBlock* NotificationMessageText = Cast<UTextBlock>(GetWidgetFromName("NotificationMessageText")))
	{
		NotificationMessageText->RemoveFromParent();
		NotificationMessageText = nullptr;
	}

	if (UVerticalBox* NotificationVBox = Cast<UVerticalBox>(GetWidgetFromName("NotificationVBox")))
	{
		//WidgetTree->RemoveWidget(NotificationVBox);
		NotificationVBox->RemoveFromParent();
		NotificationVBox = nullptr;
	}
}
