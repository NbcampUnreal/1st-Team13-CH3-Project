#include "BGameInstance.h"
#include "BUIManager.h"

UBGameInstance::UBGameInstance()
{
	UIManagerInstance = nullptr;
}

UBUIManager* UBGameInstance::GetUIManagerInstance()
{
	return UIManagerInstance;
}

void UBGameInstance::Init()
{
	UIManagerInstance = NewObject<UBUIManager>(this, UBUIManager::StaticClass());
}
