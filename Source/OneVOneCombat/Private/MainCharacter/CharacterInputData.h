// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CharacterInputData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FCharacterInputData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector rawMoveInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector rawRotateInput;
};