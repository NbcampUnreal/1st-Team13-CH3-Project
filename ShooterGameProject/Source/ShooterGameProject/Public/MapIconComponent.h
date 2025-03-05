#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MapIconComponent.generated.h"

UENUM(BlueprintType)
enum class EIconType : uint8
{
	E_None				UMETA(DisplayName = "NONE"),
	E_Player			UMETA(DisplayName = "PLAYER"),
	E_Enemy				UMETA(DisplayName = "ENEMY"),
	E_ConsumableShop	UMETA(DisplayName = "CONSUMABLESHOP"),
	E_WeaponShop		UMETA(DisplayName = "WEAPONSHOP"),
	E_Door				UMETA(DisplayName = "DOOR")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class SHOOTERGAMEPROJECT_API UMapIconComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMapIconComponent();

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void CreateIcon();

	FTimerHandle DelayTimerHandle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
	UTexture2D* IconTexture;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
