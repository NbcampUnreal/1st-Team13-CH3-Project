#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MapIconComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class SHOOTERGAMEPROJECT_API UMapIconComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMapIconComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void CreateIcon();
	UFUNCTION()
	void RemoveIcon();

	FTimerHandle DelayTimerHandle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
	UTexture2D* IconTexture;

	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
