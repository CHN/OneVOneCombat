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

	FQuat Rotation;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isGrounding;

	UPROPERTY(VisibleAnywhere)
	bool isMovementBeginApplied;

	UPROPERTY(VisibleAnywhere)
	bool constrainInputToGround;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isJumping; // Will be moved to another data group

	UPROPERTY(VisibleAnywhere)
	FVector2D inputMove; // FIXME: Move to input data struct!
};