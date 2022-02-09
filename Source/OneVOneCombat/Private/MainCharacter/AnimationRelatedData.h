// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AnimationRelatedData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FAnimationRelatedData
{
public:
	
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isRootMotionBeingUsed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector rootMotionMoveDelta;
};
