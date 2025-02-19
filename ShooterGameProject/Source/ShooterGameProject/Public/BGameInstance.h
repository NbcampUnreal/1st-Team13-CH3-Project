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
/*
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 HP;

*/

};
