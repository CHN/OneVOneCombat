// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "InputQueueOutputState.h"
#include "InputQueueAction.h"

#include "InputQueueDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UInputQueueDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UInputQueueDataAsset(const FObjectInitializer& ObjectInitializer);

	const TArray<FInputQueueAction>& GetInputActions() const;
	EInputQueueOutputState GetInputQueueOutputState() const;

private:

	UPROPERTY(EditAnywhere)
	TArray<FInputQueueAction> InputActions;

	UPROPERTY(EditAnywhere)
	EInputQueueOutputState InputQueueOutputState;

	UPROPERTY(EditAnywhere)
	float MinQueueTime;

	UPROPERTY(EditAnywhere)
	float MaxQueueTime;
};
