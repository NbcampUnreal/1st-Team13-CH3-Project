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
	void UpdateKillLog(const FName& Name);
	void UpdateKillLog_Implementation(const FName& Name);

	//UFUNCTION(BlueprintCallable)
	//void DeleteTexture();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* KillerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* KilledName;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	//class UImage* WeaponIcon;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//UTexture2D* WeaponIconTexture = nullptr;
};
