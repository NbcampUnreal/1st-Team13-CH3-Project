#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BShopItemRow.generated.h"

USTRUCT(BlueprintType)
struct FBShopItemRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	FBShopItemRow();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Info;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;
};
