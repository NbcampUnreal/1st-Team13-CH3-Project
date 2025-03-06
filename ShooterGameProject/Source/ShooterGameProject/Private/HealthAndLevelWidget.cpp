#include "HealthAndLevelWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"


void UHealthAndLevelWidget::UpdateHealth(float CurrentHP, float MaxHP)
{
	if (MaxHP <= 0 || CurrentHP < 0) return;

	// HP Numbers
	if (UTextBlock* CurrentHPText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentHPText"))))
	{
		CurrentHPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CurrentHP)));
	}
	if (UTextBlock* MaxHPText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxHPText"))))
	{
		MaxHPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), MaxHP)));
	}

	// HP Bar
	if (UProgressBar* HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar"))))
	{
		float HPPercent = CurrentHP / MaxHP;
		HPBar->SetPercent(HPPercent);
	}

	if (Anim_Jiggle)
	{
		PlayAnimation(Anim_Jiggle);
	}
}

void UHealthAndLevelWidget::UpdateLevelAndExp(int32 PlayerLevel, float CurrentExp, float MaxExp)
{
	if (MaxExp <= 0 || CurrentExp < 0 || PlayerLevel < 0) return;

	// Level
	if (UTextBlock* PlayerLevelText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerLevelText"))))
	{
		PlayerLevelText->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), PlayerLevel)));
	}

	// Exp Bar
	if (UProgressBar* ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ExpBar"))))
	{
		float ExpPercent = CurrentExp / MaxExp;
		ExpBar->SetPercent(ExpPercent);
	}
}
