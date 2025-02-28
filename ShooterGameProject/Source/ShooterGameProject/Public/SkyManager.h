#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkyAtmosphereComponent.h"
#include "SkyManager.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API ASkyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASkyManager();

protected:
	virtual void BeginPlay() override;

public:	
	void UpdateSkyColor(int CurrentStage);

private:
	USkyAtmosphereComponent* SkyComponent;
};