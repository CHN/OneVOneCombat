// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AnimationFlaggedBool.generated.h"

/**
 * 
 */

UCLASS(BlueprintType)
class ONEVONECOMBAT_API UAnimationFlaggedBool : public UObject
{
public:
	
	GENERATED_BODY()

	UAnimationFlaggedBool();

	void SetTriggerValue(bool val);

	inline bool IsJustTriggered()
	{
		return isJustTriggered;
	}

	inline bool IsAnimationContinue()
	{
		return isAnimationContinue | isJustTriggered;
	}

	inline void operator=(bool val)
	{
		SetTriggerValue(val);
	}

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool isJustTriggered;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool isAnimationContinue;

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void SetAnimationExecutedFlag(bool val);
};
