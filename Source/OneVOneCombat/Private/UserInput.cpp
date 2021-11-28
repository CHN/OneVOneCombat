// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInput.h"

FUserInput UserInputUtilities::ConvertActionToUserInput(EUserInputType inputType, EInputEvent inputEvent)
{
	FUserInput userInput;

	userInput.inputType = inputType;
	userInput.inputValue = inputEvent == EInputEvent::IE_Pressed ? 1 : -1;
	userInput.inputEvent = inputEvent;
	userInput.timeStamp = FDateTime::Now();

	return userInput;
}

float UserInputUtilities::ConvertActionToAxisInput(EInputEvent negativeInputEvent, EInputEvent positiveInputEvent)
{
	float negativeInput = negativeInputEvent == EInputEvent::IE_Pressed ? -1.f : 0.f;
	float positiveInput = positiveInputEvent == EInputEvent::IE_Pressed ? 1.f : 0.f;

	return negativeInput + positiveInput;
}

float UserInputUtilities::ConvertActionToAxisInputByBools(bool negativeInputEvent, bool positiveInputEvent)
{
	float negativeInput = negativeInputEvent ? -1.f : 0.f;
	float positiveInput = positiveInputEvent ? 1.f : 0.f;

	return negativeInput + positiveInput;
}
