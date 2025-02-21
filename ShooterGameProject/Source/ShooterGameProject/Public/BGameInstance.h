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
	UPROPERTY(BlueprintReadWrite)
	bool HasTitleScreenBeenShown = false;

protected:
	UPROPERTY()
	UBUIManager* UIManagerInstance;

public:
	UFUNCTION(BlueprintPure)
	UBUIManager* GetUIManagerInstance();


};
