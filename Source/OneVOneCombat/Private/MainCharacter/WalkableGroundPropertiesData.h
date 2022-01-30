// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WalkableGroundPropertiesData.generated.h"

/**
 * 
 */

USTRUCT()
struct FWalkableGroundPropertiesData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float minSlopeDotProduct = 0.95f;

	UPROPERTY(EditAnywhere)
	float maxSlopeDotProduct = 1.f;

	UPROPERTY(VisibleAnywhere)
	float sweepGroundAvoidanceDistance = 1.f;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> collisionChannel;
};