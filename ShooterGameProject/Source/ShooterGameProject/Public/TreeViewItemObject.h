#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemStruct.h"
#include "TreeViewItemObject.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UTreeViewItemObject : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TreeView")
	FItemData ItemData;
};
