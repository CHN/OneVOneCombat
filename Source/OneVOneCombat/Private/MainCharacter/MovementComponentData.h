// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CachedVector.h"

#include "MovementComponentData.generated.h"

USTRUCT(BlueprintType)
struct FMovementComponentData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FCachedVector gravity;

	UPROPERTY(VisibleAnywhere)
	FVector velocity;

	UPROPERTY(VisibleAnywhere)
	FVector movementTargetPosition;

	UPROPERTY(VisibleAnywhere)
	FVector movementDelta;

	UPROPERTY(VisibleAnywhere)
	float movementDuration;

	UPROPERTY(VisibleAnywhere)
	float currentDuration;

	UPROPERTY(VisibleAnywhere)
	FQuat currentRotation;

	UPROPERTY(EditAnywhere)
	float verticalRotationMinDegree;

	UPROPERTY(EditAnywhere)
	float verticalRotationMaxDegree;

	// FIXME: Booleans can be moved up to fix padding
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isGrounding;

	UPROPERTY(VisibleAnywhere)
	bool isMovementBeginApplied;

	UPROPERTY(VisibleAnywhere)
	bool constrainInputToGround;
};