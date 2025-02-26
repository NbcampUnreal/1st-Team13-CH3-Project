#include "MissionWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Blueprint/WidgetTree.h"

void UMissionWidget::UpdateItemMission(const FName& ItemName, const int32& CurrentCount, const int32& TargetCount)
{
	if (UVerticalBox* MissionVBox = Cast<UVerticalBox>(GetWidgetFromName(TEXT("MissionVBox"))))
	{
		FString TextToSet = ItemName.ToString() + FString::Printf(TEXT(" (%d / %d)"), CurrentCount, TargetCount);
		UTextBlock* MissionText = Cast<UTextBlock>(GetWidgetFromName(ItemName));
		if (MissionText == nullptr)
		{
			MissionText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), ItemName);
			if (MissionText)
			{
				if (!bIsTitleAdded)
				{
					AddMissionTitle("FIND MISSING ITEMS", MissionVBox);
					bIsTitleAdded = true;
				}
				MissionVBox->AddChildToVerticalBox(MissionText);
				TextBlocks.Add(MissionText);
			}
		}
		if (MissionText)
		{
			MissionText->SetText(FText::FromString(TextToSet));
			
			FSlateFontInfo FontInfo = MissionText->GetFont();
			FontInfo.Size = 24.f;
			FontInfo.TypefaceFontName = FName(TEXT("Light"));
			MissionText->SetFont(FontInfo);
			MissionText->SetJustification(ETextJustify::Right);

			if (CurrentCount == TargetCount)
			{
				OnMissionCompleted(MissionText);
			}
		}
	}
}

void UMissionWidget::UpdateBonusMission(const int32& CurrentCount, const int32& TargetCount)
{
	if (UVerticalBox* MissionVBox = Cast<UVerticalBox>(GetWidgetFromName(TEXT("MissionVBox"))))
	{
		UTextBlock* MissionText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Bonus")));
		// create a new UTextBlock to display mission
		if (MissionText == nullptr)
		{
			MissionText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Bonus"));
			if (MissionText)
			{
				if (!bIsTitleAdded)
				{
					AddMissionTitle("KILL ALL ENEMIES", MissionVBox);
					bIsTitleAdded = true;
				}
				MissionVBox->AddChildToVerticalBox(MissionText);
				TextBlocks.Add(MissionText);
			}
		}
		// update the text
		if (MissionText)
		{
			MissionText->SetText(FText::FromString(FString::Printf(TEXT("Kill Count (%d / %d)"), CurrentCount, TargetCount)));
			
			// Set padding
			if (UVerticalBoxSlot* MissionSlot = Cast<UVerticalBoxSlot>(MissionText->Slot))
			{
				MissionSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 10.f));
			}
			// Set typeface, size, and justification
			FSlateFontInfo FontInfo = MissionText->GetFont();
			FontInfo.Size = 24.f;
			FontInfo.TypefaceFontName = FName(TEXT("Light"));
			MissionText->SetFont(FontInfo);
			MissionText->SetJustification(ETextJustify::Right);
			
			if (CurrentCount == TargetCount)
			{
				OnMissionCompleted(MissionText);
			}
		}
	}
}

void UMissionWidget::AddMissionTitle(const FString& MissionTitle, UVerticalBox* MissionVBox)
{
	// TODO: Get the VBox and create text block as a child
	if (UTextBlock* MissionTitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), "MissionTitle"))
	{
		TextBlocks.Add(MissionTitleText);
		MissionVBox->AddChildToVerticalBox(MissionTitleText);
		MissionTitleText->SetText(FText::FromString(MissionTitle));

		// Set padding
		if (UVerticalBoxSlot* MissionTitleSlot = Cast<UVerticalBoxSlot>(MissionTitleText->Slot))
		{
			MissionTitleSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 10.f));
		}																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																						

		// Set typeface, size, and justification
		FSlateFontInfo TitleFontInfo = MissionTitleText->GetFont();
		TitleFontInfo.Size = 24.f;
		TitleFontInfo.TypefaceFontName = FName(TEXT("Regular"));
		MissionTitleText->SetFont(TitleFontInfo);
	}
}

void UMissionWidget::RemoveAllMissions()
{
	for (int32 i = 0; i < TextBlocks.Num(); i++)
	{
		if (TextBlocks[i])
		{
			TextBlocks[i]->RemoveFromParent();
			TextBlocks[i] = nullptr;
		}
	}
	TextBlocks.Empty();
	bIsTitleAdded = false;
}