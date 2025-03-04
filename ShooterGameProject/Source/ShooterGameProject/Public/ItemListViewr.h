// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemListViewr.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAMEPROJECT_API UItemListViewr : public UUserWidget
{
	GENERATED_BODY()	
public:
	virtual void NativeConstruct() override;
protected:	
	UPROPERTY(meta = (BindWidgetOptional))
	class UVerticalBox* VerticalBox;
	UPROPERTY(meta = (BindWidgetOptional))
	class UTextBlock* ListTitle;
	UPROPERTY(meta = (BindWidgetOptional))
	class UOverlay* Overlay;
	UPROPERTY(meta = (BindWidgetOptional))
	class UBorder* Border;
	UPROPERTY(meta = (BindWidgetOptional))
	class UTreeView* ItemListView;
public:
	UFUNCTION(BlueprintCallable)
	class UTreeView* GetTreeView() const { return ItemListView; }
protected:
	virtual bool NativeOnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent, UDragDropOperation* Operation) override;
private:
	class UInventoryDragDropOperation* WidgetOperation;
protected:
	void AddItem(class UTreeView* TreeView);
	void RemoveItem(class UTreeView* TreeView);

};
