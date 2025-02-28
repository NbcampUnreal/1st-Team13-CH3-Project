#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BShopManagerPawn.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API ABShopManagerPawn : public APawn
{
	GENERATED_BODY()

public:
	ABShopManagerPawn();

	// To be called when the shop should be enabled (i.e. on stage cleared)
	UFUNCTION(BlueprintCallable)
	void EnableShop();

	// Create/Remove shop menu
	UFUNCTION(BlueprintCallable)
	void OpenShopWidget();
	UFUNCTION(BlueprintCallable)
	void CloseShopWidget();

protected:
	virtual void BeginPlay() override;

	// Scene, Mesh, Collision, Light components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop|Components")
	USceneComponent* SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop|Components")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop|Components")
	USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop|Components")
	class UBoxComponent* CollisionBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop|Components")
	class UPointLightComponent* PointLight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop|Components")
	class UCameraComponent* Camera;

	// Animations for skeletal mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop|Animations")
	UAnimMontage* Montage;

	// Shop widget class and instance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop|Widget")
	TSubclassOf<UUserWidget> ShopWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop|Widget")
	UUserWidget* ShopWidgetInstance;

	// Shop lights to be turned on/off based on the game status
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop|Properties")
	float LightIntensity;

	bool bIsShopEnabled;
	bool bIsShopWidgetOpen;

	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnInteractKeyPressed();

	UFUNCTION(BlueprintCallable)
	void PlayAnim(const FName& SectionName);
};
