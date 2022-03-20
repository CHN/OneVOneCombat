// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UserInputType.generated.h"

UENUM(BlueprintType)
enum class EUserInputType : uint8
{
	ATTACK_INPUT,
	JUMP_INPUT,
	CROUCH_INPUT,
	CHANGE_WEAPON_PREVIOUS,
	CHANGE_WEAPON_NEXT,
	SPRINT,

	END_OF_ENUM
};