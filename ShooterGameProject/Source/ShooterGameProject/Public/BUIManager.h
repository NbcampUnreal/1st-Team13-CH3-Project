#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BUIManager.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UBUIManager : public UObject
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|TitleScreen")
	TSubclassOf<UUserWidget> TitleWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|TitleScreen")
	UUserWidget* TitleWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|LevelTransition")
	TSubclassOf<UUserWidget> LevelTransitionWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|LevelTransition")
	UUserWidget* LevelTransitionWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|InGameMenu")
	TSubclassOf<UUserWidget> InGameMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|InGameMenu")
	UUserWidget* InGameMenuWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|GameOver")
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|GameOver")
	UUserWidget* GameOverWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|HUD")
	UUserWidget* HUDWidgetInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|Notification")
	class UNotificationWidget* NotificationWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|Notification")
	class UItemNotificationWidget* ItemNotificationWidget;


	FTimerHandle UpdateHUDTimerHandle;

	UBUIManager();
	virtual void PostInitProperties() override;

	UFUNCTION(BlueprintPure)
	UUserWidget* GetTitleWidgetInstance();
	UFUNCTION(BlueprintPure)
	UUserWidget* GetLevelTransitionWidgetInstance();
	UFUNCTION(BlueprintPure)
	UUserWidget* GetInGameMenuWidgetInstance();
	UFUNCTION(BlueprintPure)
	UUserWidget* GetGameOverWidgetInstance();
	UFUNCTION(BlueprintPure)
	UUserWidget* GetHUDWidgetInstance();

	UFUNCTION(BlueprintCallable)
	void EnterTitleScreen();
	UFUNCTION(BlueprintCallable)
	void ExitTitleScreen();

	UFUNCTION(BlueprintCallable)
	void LevelStartTransition();
	UFUNCTION(BlueprintCallable)
	void LevelEndTransition();
	UFUNCTION(BlueprintCallable)
	void RemoveLevelTransition();

	UFUNCTION(BlueprintCallable)
	void EnterInGameMenu();
	UFUNCTION(BlueprintCallable)
	void ExitInGameMenu();

	UFUNCTION(BlueprintCallable)
	void EnterGameOverScreen();
	UFUNCTION(BlueprintCallable)
	void ExitGameOverScreen();

	void OnFire();
	UFUNCTION(BlueprintCallable)
	void DisplayHUD();
	UFUNCTION(BlueprintCallable)
	void CollapseHUD();
	void RemoveHUD();
	void UpdateHUDTimed();
	void UpdateHUDHealth(float CurrentHP, float MaxHP);
	//void UpdateHUDMission(const FString& Title, const FString& Content, int32 CurrentCount, int32 MaxCount);
	//void UpdateHUDMap();
	void UpdateHUDQuickSlot(FName ItemName, int32 Count);
	void UpdateHUDAmmo(int32 LoadedCount, int32 InventoryCount);
	void UpdateHUDEquippedWeapon(FName WeaponType);
	//void UpdateHUDStance();

	void DisplayNotification(FString Title, FString Message);
	void RemoveNotification();

	void DisplayItemNotification(FName ItemName);
	void RemoveItemNotification();

	TTuple<FVector, FVector> GetCrosshairLocationAndDirection();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Icons")
	UTexture2D* PistolIconTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Icons")
	UTexture2D* RifleIconTexture;

	class UBGameInstance* GameInstance;

	void SetInputUIOnly();
	void SetInputGameOnly();
};
