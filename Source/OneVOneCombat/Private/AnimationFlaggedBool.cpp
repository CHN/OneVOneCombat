// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationFlaggedBool.h"

UAnimationFlaggedBool::UAnimationFlaggedBool()
{
	isJustTriggered = false;
	isAnimationContinue = false;
}

void UAnimationFlaggedBool::SetTriggerValue(bool val)
{
	isJustTriggered = val;
	isAnimationContinue = false;
}

void UAnimationFlaggedBool::SetAnimationExecutedFlag(bool val)
{
	isAnimationContinue = val;

	if (isAnimationContinue)
	{
		isJustTriggered = false;
	}
}