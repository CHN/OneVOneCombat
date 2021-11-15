// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EPlayerState.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	WALK,
	JUMP,
	CROUCH,
	ATTACK,

	END_OF_ENUM
};