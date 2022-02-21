// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AnimationFlaggedBool.h"

#include "CharacterState.generated.h"

/**
 * 
 */

UCLASS(BlueprintType)
class UCharacterState : public UObject
{
public:
	GENERATED_BODY()

	UCharacterState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAnimationFlaggedBool* jumpState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAnimationFlaggedBool* swordAttackState;
};
