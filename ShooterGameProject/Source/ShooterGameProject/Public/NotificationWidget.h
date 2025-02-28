#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationWidget.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UNotificationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void DisplayNotification(const FString& Title, const FString& Message);

	UFUNCTION(BlueprintImplementableEvent)
	void FadeInOutNotification();

	UFUNCTION(BlueprintCallable)
	void RemoveNotification();
};
