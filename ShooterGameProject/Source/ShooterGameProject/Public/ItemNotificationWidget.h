#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemNotificationWidget.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UItemNotificationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UItemNotificationWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void DisplayNotification(FName ItemName);
	UFUNCTION(BlueprintImplementableEvent)
	void FadeInOutNotification();
	UFUNCTION(BlueprintCallable)
	void RemoveNotification();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Assets")
	UTexture2D* FirstAidKitIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Assets")
	UTexture2D* GrenadeIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Assets")
	TObjectPtr<UObject> RobotoItalic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Assets")
	UMaterialInstance* PurpleGlowMaterial;

	float ImageSizeX;
	float ImageSizeY;
};
