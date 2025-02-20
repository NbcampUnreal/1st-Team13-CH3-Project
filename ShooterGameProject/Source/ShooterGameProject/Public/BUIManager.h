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

	UFUNCTION(BlueprintCallable)
	void UpdateHUD();

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

protected:
	class UBGameInstance* GameInstance;

	void SetInputUIOnly();
	void SetInputGameOnly();
};
