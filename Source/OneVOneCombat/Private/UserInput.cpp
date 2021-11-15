// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInput.h"

FUserInput UserInputUtilities::ConvertActionToUserInput(EPlayerState targetState, ActionMappingState actionMapState)
{
	FUserInput userInput;

	userInput.inputValue = actionMapState == ActionMappingState::Pressed ? 1 : -1;
	userInput.state = targetState;
	userInput.timeStamp = FDateTime::Now();

	return userInput;
}