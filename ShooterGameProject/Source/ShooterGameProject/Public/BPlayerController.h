#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BPlayerController.generated.h"


UCLASS()
class SHOOTERGAMEPROJECT_API ABPlayerController : public APlayerController
{
	GENERATED_BODY()
	friend class ABCharacter;
public:
	ABPlayerController();
protected:
	virtual void BeginPlay() override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> WidgetHUDClass;
	class UUserWidget* WidgetHUDInstance;
};
