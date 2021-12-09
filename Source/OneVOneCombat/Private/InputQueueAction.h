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
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ToolTip = "Discards this queue if duration between current and previous input action time is less than this value. (If index is 0, this will be discarded.)"))
	float minPreviousInputTime = 0.f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ToolTip = "Discards this queue if duration between current and previous input action time is greater than this value. (If index is 0, this will be discarded.)"))
	float maxPreviousInputTime = 10000.f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ToolTip = "Discards this queue if input event's click, release etc. duration is greater than this value."))
	int32 expireTime = 1000.f;

	UPROPERTY(EditAnywhere, meta = (ToolTip = "Discards release input event of a pressed event automatically."))
	bool bDiscardReleaseInputOfPressEvent = true;
};
