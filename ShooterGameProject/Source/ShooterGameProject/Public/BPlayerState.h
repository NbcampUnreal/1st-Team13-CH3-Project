// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ItemStruct.h"
#include "BCharacter.h"
#include "BPlayerState.generated.h"
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(Health_DeathEvent, AActor*, OwningActor);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(Health_AttributeChanged, ULyraHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);
// Keep 
// ��� ���� ���� �𸣴� �ϴ� ŵ�ص���
UENUM(BlueprintType)
enum class EDeathState : uint8
{
	NotDead = 0 UMETA(DisplayName = "NotDead"),
	DeathStarted UMETA(DisplayName = "DeathStarted"),
	DeathFinished UMETA(DisplayName = "DeathFinished")
};
UCLASS()
class SHOOTERGAMEPROJECT_API ABPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ABPlayerState();
protected:
	float NomalSpeed;
	float SprintSpeed;
	float SprintMultiply;
	float MultiplySpeed;
	int32 MaxHealth;
	int32 CurrentHealth;
	int32 AttackDamage;
	int32 Level;
	int32 MaxExperience;
	int32 CurrentExperience;
	int32 Coin;
	EDeathState DeathState;
	TMap<FName, TArray<FItemData>> Inventory;
	TObjectPtr<ABCharacter> Player;
protected:
	virtual void BeginPlay() override;
public:
	UFUNCTION(BlueprintCallable, Category = "Coin")
	void AddCoin(const int32 Coin);
	UFUNCTION(BlueprintCallable, Category = "Coin")
	int32 GetCoin() const;
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetCurrentHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxHealth() const;
	// 0 ~ 1�� ���� (ProgressBar����)
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthNormalized() const;
	UFUNCTION(BlueprintCallable, Category = "Level")
	int32 GetPlayerLevel() const;
	UFUNCTION(BlueprintCallable, Category = "Level")
	int32 GetCurrentExp() const;
	UFUNCTION(BlueprintCallable, Category = "Level")
	int32 GetMaxExp() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Health")
	bool IsDeadOrDying() const;
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Health")
	virtual void StartDeath();
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Health")
	virtual void FinishDeath();
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Health")
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Health")
	void AddCurrentHealth(int32 Helth);
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Health")
	void AddExp(int32 Exp);
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Health")
	void Attack(AActor* Actor);
	UFUNCTION(BlueprintCallable, Category = "Collect")
	TArray<FItemData> GetNearItemArray() const;
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FItemData> GetAllInventoryItem() const;
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FItemData> GetInventoryTypeItem(const FName& ItemName) const;
	TArray<FItemData> GetInventoryClassItem(const UClass* ItemClass) const;
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InventoryRemoveItem(const FItemData& Item);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InventoryAddItem(const FItemData& Item);
	UFUNCTION(BlueprintCallable)
	void UpdateQuickSlot(const FName& ItemName, int32 Count);
	UFUNCTION(BlueprintCallable)
	void UseItem(const FName& ItemName);
private:
	void LevelUP();
	//	�ɸ��� �������� ���� ���̺�
	TMap<int32, int32> LevelTable;
};
