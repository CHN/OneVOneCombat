// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UserInputType.h"
#include "InputQueueOutputState.h"

#include "InputQueueAction.generated.h"

/**
 * 
 */

USTRUCT()
struct FInputQueueAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EUserInputType inputType;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EInputEvent> inputEvent;

	// TODO: Convert to double
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float minPreviousInputTime = 0.f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float maxPreviousInputTime = 10000.f;

	UPROPERTY(EditAnywhere)
	bool bDiscardReleaseInputOfPressEvent = true;
};
