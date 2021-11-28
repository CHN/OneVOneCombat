// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PlayerStateObject.h"

#include "UserInput.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class UserInputType : uint8
{
	ATTACK_INPUT,
	JUMP_INPUT,
	CROUCH_INPUT
};

USTRUCT(BlueprintType)
struct FUserInput
{
	UserInputType inputType;
	float inputValue;
	FDateTime timeStamp;

	GENERATED_BODY();
};

class UserInputUtilities
{
public:

	static FUserInput ConvertActionToUserInput(UserInputType inputType, EInputEvent inputEvent);
	static float ConvertActionToAxisInput(EInputEvent negativeInputEvent, EInputEvent positiveInputEvent);
	static float ConvertActionToAxisInputByBools(bool negativeInputEvent, bool positiveInputEvent);
};