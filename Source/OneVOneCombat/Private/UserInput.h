// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UserInputType.h"

#include "UserInput.generated.h"

USTRUCT(BlueprintType)
struct FUserInput
{
	GENERATED_BODY();

	UPROPERTY(VisibleAnywhere)
	EUserInputType inputType;

	UPROPERTY(VisibleAnywhere)
	float inputValue;
		
	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<EInputEvent> inputEvent;

	UPROPERTY(VisibleAnywhere)
	FDateTime timeStamp;
};

class UserInputUtilities
{
public:
		
	static FUserInput ConvertActionToUserInput(EUserInputType inputType, EInputEvent inputEvent);
	static float ConvertActionToAxisInput(EInputEvent negativeInputEvent, EInputEvent positiveInputEvent);
	static float ConvertActionToAxisInputByBools(bool negativeInputEvent, bool positiveInputEvent);
};