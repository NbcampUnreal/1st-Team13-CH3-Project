#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BUIManager.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UBUIManager : public UObject
{
	GENERATED_BODY()
	
public:	
	UBUIManager();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|WeaponWheel")
	TSubclassOf<UUserWidget> WeaponWheelClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|WeaponWheel")
	UUserWidget* WeaponWheelInstance;

	/* UUserWidget objects in HUD */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|HUDWidgets")
	class UNotificationWidget* NotificationWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|HUDWidgets")
	class UItemNotificationWidget* ItemNotificationWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|HUDWidgets")
	class UHealthAndLevelWidget* HealthAndLevelWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|HUDWidgets")
	class UMapWidget* MapWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|HUDWidgets")
	class UMissionWidget* MissionWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|HUDWidgets")
	class UQuickslotWidget* QuickslotWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|HUDWidgets")
	class UWeaponAmmoWidget* WeaponAmmoWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|HUDWidgets")
	class UCrosshairWidget* CrosshairWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|HUDWidgets")
	class UKillLogWidget* KillLogWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|HUDWidgets")
	class UKillCountWidget* KillCountWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|Inventory")
	TSubclassOf<UUserWidget> InventoryWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|Inventory")
	class UBInventoryWidget* InventoryWidgetInstance;

	FTimerHandle UpdateHUDTimerHandle;
	bool bIsWeaponWheelOpen;

	/* Getter functions for UUserWidget instances */
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
	UFUNCTION(BlueprintPure)
	UUserWidget* GetWeaponWheelInstance();
	UFUNCTION(BlueprintPure)
	UUserWidget* GetInventoryInstance();
	/* Title Screen */
	UFUNCTION(BlueprintCallable)
	void EnterTitleScreen();
	UFUNCTION(BlueprintCallable)
	void ExitTitleScreen();

	/* Level Transition */
	UFUNCTION(BlueprintCallable)
	void LevelStartTransition();
	UFUNCTION(BlueprintCallable)
	void LevelEndTransition();
	UFUNCTION(BlueprintCallable)
	void RemoveLevelTransition();

	/* In-Game Menu */
	UFUNCTION(BlueprintCallable)
	void EnterInGameMenu();
	UFUNCTION(BlueprintCallable)
	void ExitInGameMenu();

	/* Game Over Screen*/
	UFUNCTION(BlueprintCallable)
	void EnterGameOverScreen();
	UFUNCTION(BlueprintCallable)
	void ExitGameOverScreen();

	/* Weapon Wheel */
	UFUNCTION(BlueprintCallable)
	void EnterWeaponWheel();
	UFUNCTION(BlueprintCallable)
	void ExitWeaponWheel();
	UFUNCTION(BlueprintCallable)
	void RemoveWeaponWheel();

	/* HUD */
	void DisplayHUD();
	void UpdateHUD();
	void CollapseHUD();
	void RemoveHUD();
	UFUNCTION(BlueprintCallable)
	void ShowInventory();
	UFUNCTION(BlueprintCallable)
	void CloseInventory();
	void UpdateHUDHealth(const float& CurrentHP, const float& MaxHP);
	void UpdateHUDLevelAndExp(const int32& PlayerLevel, const float& CurrentExp, const float& MaxExp);
	void UpdateHUDQuickSlot(const FName& ItemName, const int32& Count);
	void UpdateHUDLoadedAmmo(const int32& LoadedAmmo = -1);
	void UpdateHUDInventoryAmmo(const int32& InventoryAmmo = -1);
	void UpdateHUDEquippedWeapon(const FString& WeaponType);
	void UpdateHUDTimed();
	void UpdateHUDMap();
	void UpdateHUDItemMission(const FName& ItemName, const int32& CurrentCount, const int32& TargetCount);
	void UpdateHUDBonusMission(const int32& CurrentCount, const int32& TargetCount);
	void RemoveAllMissions();
	void UpdateKillLog(const FName& KilledName);
	void UpdateKillCount(const int32& KillCount);
	void UpdateCurrentScore(const int32& CurrentScore);
	void OnFire(); // Crosshair animation when fired
	void OnNormalHit(); // Hit Marker animation upon successful hit

	/* Notification (Top Center) / Added Item Notification (Left Center) */
	void DisplayNotification(const FString& Title, const FString& Message);
	void DisplayItemNotification(const FName& ItemName);

	/* Crosshair Location and Direction for weapons */
	TTuple<FVector, FVector> GetCrosshairLocationAndDirection();

protected:
	/* Owning Game Instance */
	class UBGameInstance* GameInstance;

	/* Switch Input Mode and Mouse Cursor Visibility */
	void SetInputUIOnly();
	void SetInputGameOnly();
	void SetInputUIAndGame();
};
