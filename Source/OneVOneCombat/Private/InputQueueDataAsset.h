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
	const FString& GetCommand() const;
	float GetMinQueueTime() const { return minQueueTime; }
	float GetMaxQueueTime() const { return maxQueueTime; }
	bool GetRemoveFromPollWhenInputQueueFound() const { return bRemoveFromPollWhenInputQueueFound; }

private:

	UPROPERTY(EditAnywhere)
	TArray<FInputQueueAction> inputActions;

	UPROPERTY(EditAnywhere)
	FString command;

	UPROPERTY(EditAnywhere)
	float minQueueTime;

	UPROPERTY(EditAnywhere)
	float maxQueueTime;

	UPROPERTY(EditAnywhere)
	bool bRemoveFromPollWhenInputQueueFound = true;
};
