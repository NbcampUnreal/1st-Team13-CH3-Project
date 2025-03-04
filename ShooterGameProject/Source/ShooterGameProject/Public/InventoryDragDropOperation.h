// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAMEPROJECT_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	friend class UTreeItem;
	friend class UItemListViewr;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* WidgetReference;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D DragOffset;
};
