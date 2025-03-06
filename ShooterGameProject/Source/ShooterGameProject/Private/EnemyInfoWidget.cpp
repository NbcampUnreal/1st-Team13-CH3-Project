#include "EnemyInfoWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEnemyInfoWidget::UpdateEnemyInfo(const FName& EnemyType, const float& CurrentHP, const float& MaxHP)
{
	if (EnemyHPBar)
	{
		float fraction = CurrentHP / MaxHP;
		EnemyHPBar->SetPercent(fraction);
	}

	if (EnemyNameText)
	{
		EnemyNameText->SetText(FText::FromName(EnemyType));
	}
}
