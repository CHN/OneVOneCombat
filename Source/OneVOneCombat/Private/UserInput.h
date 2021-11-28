// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UserInputType.h"

#include "UserInput.generated.h"

USTRUCT(BlueprintType)
struct FUserInput
{
	EUserInputType inputType;
	float inputValue;
	TEnumAsByte<EInputEvent> inputEvent;
	FDateTime timeStamp;

	GENERATED_BODY();
};

class UserInputUtilities
{
public:

	static FUserInput ConvertActionToUserInput(EUserInputType inputType, EInputEvent inputEvent);
	static float ConvertActionToAxisInput(EInputEvent negativeInputEvent, EInputEvent positiveInputEvent);
	static float ConvertActionToAxisInputByBools(bool negativeInputEvent, bool positiveInputEvent);
};