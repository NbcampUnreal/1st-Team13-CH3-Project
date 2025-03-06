#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillLogWidget.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UKillLogWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void UpdateKillLog(const FName& Name, UTexture2D* IconTexture);
	void UpdateKillLog_Implementation(const FName& Name, UTexture2D* IconTexture);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* KillerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* KilledName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* WeaponIcon;
};
