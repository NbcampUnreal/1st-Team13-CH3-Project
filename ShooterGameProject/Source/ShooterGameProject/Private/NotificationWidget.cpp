#include "NotificationWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Blueprint/WidgetTree.h"

void UNotificationWidget::DisplayNotification(const FString& Title, const FString& Message)
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
			NotificationTitleText->SetJustification(ETextJustify::Center);

			FSlateFontInfo FontInfo = NotificationTitleText->GetFont();
			FontInfo.TypefaceFontName = "Regular";
			FontInfo.Size = 24.f;
			NotificationTitleText->SetFont(FontInfo);
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
			NotificationMessageText->SetJustification(ETextJustify::Center);

			FSlateFontInfo FontInfo = NotificationMessageText->GetFont();
			FontInfo.TypefaceFontName = "Light";
			FontInfo.Size = 24.f;
			NotificationMessageText->SetFont(FontInfo);
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

	//if (UVerticalBox* NotificationVBox = Cast<UVerticalBox>(GetWidgetFromName("NotificationVBox")))
	//{
		//WidgetTree->RemoveWidget(NotificationVBox);
		//NotificationVBox->RemoveFromParent();
		//NotificationVBox = nullptr;
	//}
}
