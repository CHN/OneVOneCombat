// Fill out your copyright notice in the Description page of Project Settings.


#include "UserActionAndAxisInputHandler.h"
#include "Misc/App.h"

void UUserActionAndAxisInputHandler::HandleXAxisNegativeInputPressed()
{
	axisActionValue.X = FMath::Max(axisActionValue.X - 1, -1);
	axisHandler.Execute(axisActionValue);
}

void UUserActionAndAxisInputHandler::HandleXAxisNegativeInputReleased()
{
	axisActionValue.X = FMath::Min(axisActionValue.X + 1, 1);
	axisHandler.Execute(axisActionValue);
}

void UUserActionAndAxisInputHandler::HandleXAxisPositiveInputPressed()
{
	axisActionValue.X = FMath::Min(axisActionValue.X + 1, 1);
	axisHandler.Execute(axisActionValue);
}

void UUserActionAndAxisInputHandler::HandleXAxisPositiveInputReleased()
{
	axisActionValue.X = FMath::Max(axisActionValue.X - 1, -1);
	axisHandler.Execute(axisActionValue);
}

void UUserActionAndAxisInputHandler::HandleYAxisNegativeInputPressed()
{
	axisActionValue.Y = FMath::Max(axisActionValue.Y - 1, -1);
	axisHandler.Execute(axisActionValue);
}

void UUserActionAndAxisInputHandler::HandleYAxisNegativeInputReleased()
{
	axisActionValue.Y = FMath::Min(axisActionValue.Y + 1, 1);
	axisHandler.Execute(axisActionValue);
}

void UUserActionAndAxisInputHandler::HandleYAxisPositiveInputPressed()
{
	axisActionValue.Y = FMath::Min(axisActionValue.Y + 1, 1);
	axisHandler.Execute(axisActionValue);
}

void UUserActionAndAxisInputHandler::HandleYAxisPositiveInputReleased()
{
	axisActionValue.Y = FMath::Max(axisActionValue.Y - 1, -1);
	axisHandler.Execute(axisActionValue);
}

void UUserActionAndAxisInputHandler::HandleXAxisInput(float value)
{
	if (FMath::Abs(axisValue.X) < 1.f)
	{
		axisValue.X += axisActionValue.X + value;
	}

	axisHandler.Execute(axisValue);
}

void UUserActionAndAxisInputHandler::HandleYAxisInput(float value)
{
	if (FMath::Abs(axisValue.Y) < 1.f)
	{
		axisValue.Y += axisActionValue.Y + value;
	}

	axisHandler.Execute(axisValue);
}

void UUserActionAndAxisInputHandler::HandleXAxisInputWithDeltaTime(float value)
{
	if (FMath::Abs(axisValue.X) < 1.f)
	{
		axisValue.X += axisActionValue.X + value * FApp::GetDeltaTime();
	}

	axisHandler.Execute(axisValue);
}

void UUserActionAndAxisInputHandler::HandleYAxisInputWithDeltaTime(float value)
{
	if (FMath::Abs(axisValue.Y) < 1.f)
	{
		axisValue.Y += axisActionValue.Y + value * FApp::GetDeltaTime();
	}

	axisHandler.Execute(axisValue);
}

void UUserActionAndAxisInputHandler::ResetAxisAccumulation()
{
	axisValue = FVector::ZeroVector;
}