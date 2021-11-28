// Fill out your copyright notice in the Description page of Project Settings.


#include "UserActionAndAxisInputHandler.h"

void UUserActionAndAxisInputHandler::HandleNegativeInputPressed()
{
	bIsNegativePressed = true;

	if (axisHandler.IsValid())
	{
		axisHandler->Execute(GetAxisValue());
	}
}

void UUserActionAndAxisInputHandler::HandleNegativeInputReleased()
{
	bIsNegativePressed = false;

	if (axisHandler.IsValid())
	{
		axisHandler->Execute(GetAxisValue());
	}
}

void UUserActionAndAxisInputHandler::HandlePositiveInputPressed()
{
	bIsPositivePressed = true;

	if (axisHandler.IsValid())
	{
		axisHandler->Execute(GetAxisValue());
	}
}

void UUserActionAndAxisInputHandler::HandlePositiveInputReleased()
{
	bIsPositivePressed = false;

	if (axisHandler.IsValid())
	{
		axisHandler->Execute(GetAxisValue());
	}
}

void UUserActionAndAxisInputHandler::HandleAxisInput(float value)
{
	if (axisHandler.IsValid())
	{
		const float resultAxisInput = GetAxisValue() + value;

		axisHandler->Execute(FMath::Clamp(resultAxisInput, -1.f, 1.f));
	}
}