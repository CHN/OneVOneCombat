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

	UPROPERTY(EditDefaultsOnly)
	float minSlopeDotProduct = 0.95f;

	UPROPERTY(EditDefaultsOnly)
	float maxSlopeDotProduct = 1.f;

	UPROPERTY(EditDefaultsOnly)
	float sweepGroundAvoidanceDistance = 1.f;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ECollisionChannel> collisionChannel;
};