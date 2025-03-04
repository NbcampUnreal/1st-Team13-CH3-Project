// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Blueprint/UserWidget.h"
#include "TreeItem.h"
#include "SendItemData.h"
#include "BInventoryWidget.generated.h"

class UItemListViewr;

UCLASS()
class SHOOTERGAMEPROJECT_API UBInventoryWidget : public UUserWidget, public ISendItemData
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct();
protected:
	UPROPERTY(meta = (BindWidgetOptional))
	UItemListViewr* NearList;
	UPROPERTY(meta = (BindWidgetOptional))
	UItemListViewr* InventoryList;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSoftClassPtr<UTreeItem> TreeItemWidgetClass;
public:
	virtual void SendItemData(const TArray<class ABBaseItem*>& ActicvatedItem, class ABPlayerState* PlayerState) override;
	UFUNCTION(BlueprintCallable)
	UItemListViewr* GetNearListTreeView() const;
	UFUNCTION(BlueprintCallable)
	UItemListViewr* GetInventoryListTreeView() const;
	UFUNCTION(BlueprintCallable)
	void AddNearList(const TArray<class ABBaseItem*>& NearItemList);
	UFUNCTION(BlueprintCallable)
	void AddInventoryList(class ABPlayerState* PlayerState);
	void AddNearTestList();
};
