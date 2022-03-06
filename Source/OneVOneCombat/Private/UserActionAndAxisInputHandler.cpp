// Fill out your copyright notice in the Description page of Project Settings.


#include "UserActionAndAxisInputHandler.h"

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
	axisValue += GetAxisValue() + value;
	axisHandler.Execute(FMath::Clamp(axisValue, -1.f, 1.f));
}

void UUserActionAndAxisInputHandler::ResetAxisAccumulation()
{
	axisValue = 0.f;
}
