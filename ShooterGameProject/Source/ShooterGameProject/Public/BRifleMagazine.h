// BRifleMagazine.h
#pragma once
#include "CoreMinimal.h"
#include "BBaseItem.h"
#include "BCharacter.h"
#include "BRifleMagazine.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API ABRifleMagazine : public ABBaseItem
{
    GENERATED_BODY()
public:
    ABRifleMagazine();
    UFUNCTION(BlueprintCallable, Category = "Item")
    virtual void UseItem(AActor* Activator) override;

private:
    int32 AmmoAmount;
    int32 AmmoCount;
};

