#include "KillCountWidget.h"
#include "Components/TextBlock.h"

void UKillCountWidget::UpdateKillCount(const int32& KillCount)
{
	if (KillCountText)
	{
		KillCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), KillCount)));
	}
}

void UKillCountWidget::UpdateCurrentScore(const int32& CurrentScore)
{
	if (CurrentScoreText)
	{
		CurrentScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurrentScore)));
	}
}
