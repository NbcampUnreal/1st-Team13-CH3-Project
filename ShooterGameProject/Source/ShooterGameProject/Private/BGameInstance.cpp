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
}

UBUIManager* UBGameInstance::GetUIManagerInstance()
{
	return UIManagerInstance;
}