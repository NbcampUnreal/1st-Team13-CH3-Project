// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ItemStruct.h"
#include "TreeItem.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API UTreeItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable)
	void SetItemName(const FName& Name);
	UFUNCTION(BlueprintCallable)
	void SetItemIcon(UTexture2D* Icon);
	UFUNCTION(BlueprintCallable)
	void SetItemType(UClass* Class);
	UFUNCTION(BlueprintCallable)
	FName GetItemName() const;
	UFUNCTION(BlueprintCallable)
	UTexture2D* GetItemIcon() const;
	UFUNCTION(BlueprintCallable)
	UClass* GetItemClass() const;
protected:
	class TreeView* OnwerTreeView;
	UPROPERTY(BlueprintReadWrite, Meta=(ExposeOnSpawn = true))
	FItemData ItemData;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ItemName;
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemIcon;
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation);
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	FReply CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey);
};
