// Fill out your copyright notice in the Description page of Project Settings.


#include "UserActionAndAxisInputHandler.h"
#include "Misc/App.h"

void UUserActionAndAxisInputHandler::HandleNegativeInputPressed()
{
	bIsNegativePressed = true;

	axisHandler.Execute(GetAxisValue());
}

void UUserActionAndAxisInputHandler::HandleNegativeInputReleased()
{
	bIsNegativePressed = false;

	axisHandler.Execute(GetAxisValue());
}

void UUserActionAndAxisInputHandler::HandlePositiveInputPressed()
{
	bIsPositivePressed = true;

	axisHandler.Execute(GetAxisValue());
}

void UUserActionAndAxisInputHandler::HandlePositiveInputReleased()
{
	bIsPositivePressed = false;

	axisHandler.Execute(GetAxisValue());
}

void UUserActionAndAxisInputHandler::HandleAxisInput(float value)
{
	if (FMath::Abs(axisValue) < 1.f)
	{
		axisValue += GetAxisValue() + value;
	}

	axisHandler.Execute(axisValue);
}

void UUserActionAndAxisInputHandler::HandleAxisInputWithDeltaTime(float value)
{
	if (FMath::Abs(axisValue) < 1.f)
	{
		axisValue += GetAxisValue() + value * FApp::GetDeltaTime();
	}

	axisHandler.Execute(axisValue);
}

void UUserActionAndAxisInputHandler::ResetAxisAccumulation()
{
	axisValue = 0.f;
}
