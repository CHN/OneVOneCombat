// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AnimationStateEventTableRow.generated.h"

/**
 * 
 */

UENUM()
enum class EAnimationStateEventType : uint8
{
	Entry,
	Exit
};

USTRUCT()
struct FAnimationStateEventTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	EAnimationStateEventType animationStateEventType;

	UPROPERTY(EditDefaultsOnly)
	FName machineName;

	UPROPERTY(EditDefaultsOnly)
	FName stateName;
};
