// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <map>
#include "CharacterEvents.generated.h"

/**
 * 
 */

struct FAnimNode_StateMachine;

DECLARE_EVENT_TwoParams(UCharacterEvents, FAnimationStateEvent, const FName& /*Machine Name*/, const FName& /*State Name*/);

UCLASS(BlueprintType)
class UCharacterEvents : public UObject
{
	GENERATED_BODY()

public:

	// FIXME: To use subscript initializer std::map was used, it should be replaced with enums and nested TArrays
	std::map<FName, std::map<FName, FAnimationStateEvent>> animationStateEntryEvents;
	std::map<FName, std::map<FName, FAnimationStateEvent>> animationStateExitEvents;
};
