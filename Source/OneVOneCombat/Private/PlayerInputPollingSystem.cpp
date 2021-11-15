// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInputPollingSystem.h"

#include "UserInput.h"

UPlayerInputPollingSystem::UPlayerInputPollingSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UPlayerInputPollingSystem::AddActionToUserInputPollingQueue(EPlayerState targetState, TEnumAsByte<ActionMappingState> actionMapState)
{
	FUserInput userInput = UserInputUtilities::ConvertActionToUserInput(targetState, actionMapState);

	inputPoll.Enqueue(std::move(userInput));
	++currentPollCount;

	if (currentPollCount == maxPollSize)
	{
		inputPoll.Pop();
		--currentPollCount;
	}
}
