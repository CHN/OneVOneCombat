// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PlayerStateObject.h"

#include "UserInput.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ActionMappingState : uint8
{
	Pressed,
	Released
};

UENUM(BlueprintType)
enum class UserInputType : uint8
{
	ATTACK,
	JUMP,
	CROUCH
};

USTRUCT(BlueprintType)
struct FUserInput
{
	EPlayerState state;
	float inputValue;
	FDateTime timeStamp;

	GENERATED_BODY();
};

class UserInputUtilities
{
public:

	static FUserInput ConvertActionToUserInput(EPlayerState targetState, ActionMappingState actionMapState);
};