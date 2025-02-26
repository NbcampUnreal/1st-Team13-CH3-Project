#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BKeyBox.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API ABKeyBox : public AActor
{
	GENERATED_BODY()
	
public:	
	ABKeyBox();

protected:
	virtual void BeginPlay() override;

public:	
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* MeshComponent;
};
