// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInputPollingSystem.h"

UPlayerInputPollingSystem::UPlayerInputPollingSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UPlayerInputPollingSystem::AddActionToUserInputPollingQueue(EUserInputType inputType, TEnumAsByte<EInputEvent> inputEvent)
{
	if (inputPoll.Num() == maxPollSize - 1)
	{
		inputPoll.RemoveAt(inputPoll.Num() - 1);
	}

	FUserInput userInput = UserInputUtilities::ConvertActionToUserInput(inputType, inputEvent);
	inputPoll.Insert(std::move(userInput), 0);

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