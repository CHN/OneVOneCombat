// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CachedVector.h"

#include "MovementComponentData.generated.h"

USTRUCT()
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

	UPROPERTY(VisibleAnywhere)
	bool isGrounding;

	UPROPERTY(VisibleAnywhere)
	bool isMovementBeginApplied;

	UPROPERTY(VisibleAnywhere)
	bool constrainInputToGround;
};