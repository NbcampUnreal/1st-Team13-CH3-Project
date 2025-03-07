#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BKeyBox.generated.h"
UENUM(BlueprintType)
enum class EBoxItemType : uint8
{
    Ammo,
    HealthKit,
    Weapon,
    Key,
    Grenade,
    None // ²Î
};
UCLASS()
class SHOOTERGAMEPROJECT_API ABKeyBox : public AActor
{
	GENERATED_BODY()
	
public:	
	ABKeyBox();

protected:
	virtual void BeginPlay() override;

public:	
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USceneComponent* RootSceneComponent;
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* MeshComponent;
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UBoxComponent* CollisionBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "KeyBox")
    float OpenDuration;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KeyBox")
    bool bIsOpening;

    FTimerHandle OpenTimerHandle;
    FTimerHandle ProgressUpdateTimer;

    UFUNCTION(BlueprintCallable, Category = "KeyBox")
    void StartOpenKeyBox();
    void CompleteOpenKeyBox();
    UFUNCTION(BlueprintCallable, Category = "KeyBox")
    void CancelOpenKeyBox();
    UFUNCTION()
    void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    EBoxItemType RandomReward();
    void SpawnAndAddItemToInventory(APlayerController* PlayerController, const FString& ItemPath);
    void GrantItemToPlayer(EBoxItemType ItemType);


    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> OpenBoxWidgetClass;
    UPROPERTY()
    UUserWidget* OpenBoxWidget;
    void UpdateProgressBar();
};
