// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BPlayerState.generated.h"

UCLASS()
class SHOOTERGAMEPROJECT_API ABPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ABPlayerState();
protected:
	virtual void BeginPlay() override;
};
