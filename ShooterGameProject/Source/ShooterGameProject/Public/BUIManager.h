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
	//void UpdateHUDStatus();
	//void UpdateHUDLevelTimer();
	//void UpdateHUDMission(const FString& Title, const FString& Content, int32 CurrentCount, int32 MaxCount);
	//void UpdateHUDMap();
	void UpdateHUDQuickSlot(FName ItemType, int32 Count);
	void UpdateHUDAmmo(int32 LoadedCount, int32 InventoryCount);
	void UpdateHUDEquippedWeapon(FName WeaponType);
	//void UpdateHUDStance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> TitleWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UUserWidget* TitleWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LevelTransitionWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UUserWidget* LevelTransitionWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> InGameMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UUserWidget* InGameMenuWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UUserWidget* GameOverWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UUserWidget* HUDWidgetInstance;

	FTimerHandle UpdateHUDTimerHandle;

protected:
	class UBGameInstance* GameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Icons")
	UTexture2D* PistolIconTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Icons")
	UTexture2D* RifleIconTexture;

	void SetInputUIOnly();
	void SetInputGameOnly();

	
};
