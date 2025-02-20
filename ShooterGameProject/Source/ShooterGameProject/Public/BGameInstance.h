#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BUIManager.h"
#include "BGameInstance.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UBGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UBGameInstance();
	virtual void Init() override;

	UPROPERTY()
	UBUIManager* UIManagerInstance;

	UFUNCTION(BlueprintPure)
	UBUIManager* GetUIManagerInstance();


};
