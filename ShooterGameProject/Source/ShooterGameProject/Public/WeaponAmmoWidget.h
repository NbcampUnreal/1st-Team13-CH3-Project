#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponAmmoWidget.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UWeaponAmmoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//UWeaponAmmoWidget(const FObjectInitializer& ObjectInitializer);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Textures")
	//UTexture2D* PistolIconTexture;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Textures")
	//UTexture2D* RifleIconTexture;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Textures")
	//UTexture2D* ShotgunIconTexture;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Textures")
	//UTexture2D* GrenadeIconTexture;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Textures")
	//UTexture2D* MeleeIconTexture;

	void UpdateWeapon(UTexture2D* WeaponTexture);
	void UpdateLoadedAmmo(const int32& LoadedAmmo = -1);
	void UpdateInventoryAmmo(const int32& InventoryAmmo = -1);

};
