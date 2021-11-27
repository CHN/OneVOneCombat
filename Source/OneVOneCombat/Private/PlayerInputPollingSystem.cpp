// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInputPollingSystem.h"

#include "UserInput.h"

UPlayerInputPollingSystem::UPlayerInputPollingSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UPlayerInputPollingSystem::AddActionToUserInputPollingQueue(UserInputType inputType, TEnumAsByte<ActionMappingState> actionMapState)
{
	FUserInput userInput = UserInputUtilities::ConvertActionToUserInput(inputType, actionMapState);

	inputPoll.Insert(std::move(userInput), 0);

	if (inputPoll.Num() == maxPollSize)
	{
		inputPoll.RemoveAt(inputPoll.Num() - 1);
	}

	onAnInputTriggered.ExecuteIfBound(this);
}

const TArray<FUserInput>& UPlayerInputPollingSystem::GetInputPoll() const
{
	return inputPoll;
}

void UPlayerInputPollingSystem::RemoveFromPolling(int32 inputSequenceCount)
{
	inputPoll.RemoveAt(0, inputSequenceCount);
}