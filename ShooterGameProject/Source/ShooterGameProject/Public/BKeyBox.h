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
    int32 KeyCount;

    FTimerHandle OpenTimerHandle;

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
    void GrantItemToPlayer(EBoxItemType ItemType);

};
