#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BGameInstance.generated.h"


UCLASS()
class SHOOTERGAMEPROJECT_API UBGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UBGameInstance();

	UPROPERTY()
	class UBUIManager* UIManagerInstance;

	UFUNCTION(BlueprintPure)
	UBUIManager* GetUIManagerInstance();

protected:
	virtual void Init() override;

};
