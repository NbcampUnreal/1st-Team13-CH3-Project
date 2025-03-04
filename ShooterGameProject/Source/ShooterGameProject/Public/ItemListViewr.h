// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemListViewr.generated.h"

UENUM()
enum class ETreeViewType
{
	NearItem  UMETA(DisplayName = "NearItem"),
	Inventory UMETA(DisplayName = "Inventory")
};
UCLASS()
class SHOOTERGAMEPROJECT_API UItemListViewr : public UUserWidget
{
	GENERATED_BODY()	
public:
	virtual void NativeConstruct() override;
	void SetTitle(const FText& Text);
	void SetTreeType(const ETreeViewType& Type);
protected:	
	UPROPERTY(meta = (BindWidgetOptional))
	class UTextBlock* ListTitle;
	UPROPERTY(meta = (BindWidgetOptional))
	class UTreeView* ItemListView;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSoftClassPtr<UUserWidget> TreeItemWidgetClass;
	ETreeViewType TreeType;

public:
	UFUNCTION(BlueprintCallable)
	class UTreeView* GetTreeView() const { return ItemListView; }
protected:
	virtual bool NativeOnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent, UDragDropOperation* Operation) override;
private:
	class UInventoryDragDropOperation* WidgetOperation;
public:
	void AddItem(class UTreeView* TreeView, class UTreeItem* TreeItem);
	void RemoveItem(class UTreeView* TreeView, class UTreeItem* TreeItem);
	void DestroyItem(class ABBaseItem* Item);
	void DropItem(UClass* Class);
	//void InputInventory()	
};
