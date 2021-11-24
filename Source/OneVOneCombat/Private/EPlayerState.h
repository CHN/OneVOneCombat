// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EPlayerState.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	FREE_STATE,
	WALK_PLAYER_STATE,
	JUMP_PLAYER_STATE,
	CROUCH_PLAYER_STATE,
	ATTACK_PLAYER_STATE,

	END_OF_ENUM
};