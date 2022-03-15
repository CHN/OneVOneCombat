// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CharacterStateData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FCharacterStateData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isJumping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isJumpingAnimationActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isQuickItemChanging;
};
