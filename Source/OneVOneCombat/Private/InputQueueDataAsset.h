// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "UserInput.h"
#include "EPlayerState.h"

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

	const TArray<UserInputType>& GetInputSequence() const;
	EPlayerState GetTargetPlayerState() const;

private:

	UPROPERTY(EditAnywhere)
	TArray<UserInputType> inputSequence;

	UPROPERTY(EditAnywhere)
	EPlayerState targetPlayerState;

	UPROPERTY(EditAnywhere)
	float minComboTime;

	UPROPERTY(EditAnywhere)
	float maxComboTime;

	UPROPERTY(EditAnywhere)
	bool fireStateImmediately;

	UPROPERTY(EditAnywhere)
	bool askStateSwitchToCurrentState = true;

	UPROPERTY(EditAnywhere)
	bool askStateSwitchToNextState = true;
};
