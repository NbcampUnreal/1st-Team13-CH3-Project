#include "BGameInstance.h"
#include "BUIManager.h"

UBGameInstance::UBGameInstance()
{
	UIManagerInstance = nullptr;
}
void UBGameInstance::Init()
{
	Super::Init();
	UIManagerInstance = NewObject<UBUIManager>(this, UBUIManager::StaticClass());
	TotalScore = 0;
}

UBUIManager* UBGameInstance::GetUIManagerInstance()
{
	return UIManagerInstance;
}

void UBGameInstance::AddScore(int32 Points)
{
	TotalScore += Points;
	GetUIManagerInstance()->UpdateCurrentScore(TotalScore);
}

int32 UBGameInstance::GetScore() const
{
	return TotalScore;
}