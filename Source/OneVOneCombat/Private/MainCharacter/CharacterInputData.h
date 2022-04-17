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

	UPROPERTY(EditAnywhere)
	bool useRootMotion;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector rawMoveInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector rawRotateInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector scaledMoveInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector scaledRotateInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D rotateSensitivity;
};