// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "UserInput.h"
#include "EPlayerState.h"

#include "ComboDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UComboDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UComboDataAsset(const FObjectInitializer& ObjectInitializer);

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
