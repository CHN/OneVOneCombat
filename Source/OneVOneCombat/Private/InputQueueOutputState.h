// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputQueueOutputState.generated.h"

UENUM(BlueprintType)
enum class EInputQueueOutputState : uint8
{
	WALK,
	JUMP,
	CROUCH,
	MELEE_ATTACK,

	END_OF_ENUM
};