// Fill out your copyright notice in the Description page of Project Settings.


#include "UserActionInputHandler.h"

void UUserActionInputHandler::HandleNegativeInputPressed()
{
	bIsNegativePressed = true;

	if (axisHandler.IsValid())
	{
		axisHandler->Execute(GetAxisValue());
	}
}

void UUserActionInputHandler::HandleNegativeInputReleased()
{
	bIsNegativePressed = false;

	if (axisHandler.IsValid())
	{
		axisHandler->Execute(GetAxisValue());
	}
}

void UUserActionInputHandler::HandlePositiveInputPressed()
{
	bIsPositivePressed = true;

	if (axisHandler.IsValid())
	{
		axisHandler->Execute(GetAxisValue());
	}
}

void UUserActionInputHandler::HandlePositiveInputReleased()
{
	bIsPositivePressed = false;

	if (axisHandler.IsValid())
	{
		axisHandler->Execute(GetAxisValue());
	}
}

void UUserActionInputHandler::HandleAxisInput(float value)
{
	if (axisHandler.IsValid())
	{
		const float resultAxisInput = GetAxisValue() + value;

		axisHandler->Execute(FMath::Clamp(resultAxisInput, -1.f, 1.f));
	}
}
