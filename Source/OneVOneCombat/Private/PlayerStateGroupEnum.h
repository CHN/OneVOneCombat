// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStateGroupEnum.generated.h"

/**
 * 
 */

UENUM()
enum class EPlayerStateGroup : uint8
{
	DEFAULT_GROUP,
	MELEE_ATTACK,
	WEAPON_ATTACK,

	END_OF_ENUM
};