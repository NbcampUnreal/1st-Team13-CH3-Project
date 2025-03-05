#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SendItemData.generated.h"

UINTERFACE(MinimalAPI)
class USendItemData : public UInterface
{
	GENERATED_BODY()
};

class SHOOTERGAMEPROJECT_API ISendItemData
{
	GENERATED_BODY()
public:
	virtual void SendItemData(const TArray<class ABBaseItem*>& ActicvatedItem, class ABPlayerState* PlayerState) = 0;
};
