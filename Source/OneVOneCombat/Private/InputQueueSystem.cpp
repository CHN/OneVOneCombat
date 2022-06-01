// Fill out your copyright notice in the Description page of Project Settings.


#include "InputQueueSystem.h"
#include "CommandMap.h"
#include "InputQueueDataAsset.h"
#include "UserInput.h"

#include "EditorUtilities.h"

// Sets default values for this component's properties
UInputQueueSystem::UInputQueueSystem()
{
	PrimaryComponentTick.bCanEverTick = false;

	commandMap = CreateDefaultSubobject<UCommandMap>("Command Map");
}

bool UInputQueueSystem::ConsumeInputs()
{
	if (commandPoll.IsEmpty())
	{
		return false;
	}

	if (WillCurrentInputBeDiscarded(commandPoll[0]))
	{
		commandPoll.RemoveAt(0, 1);
		return false;
	}

	TArray<UInputQueueDataAsset*> inputQueueDataCandidates;

	for (UInputQueueDataAsset* const inputQueueDataAsset : inputQueueDataArray)
	{
		if (inputQueueDataAsset->GetInputActions().Num() <= commandPoll.Num())
		{
			inputQueueDataCandidates.Add(inputQueueDataAsset);
		}
	}

	if (inputQueueDataCandidates.IsEmpty())
	{
		return false;
	}

	for (int32 i = 0; i < commandPoll.Num(); ++i)
	{
		const FUserInput& currentUserInput = commandPoll[i];

		double lastTwoInputInterval = 0.0;

		if (commandPoll.Num() > 1 && i < commandPoll.Num() - 1)
		{
			const FUserInput& afterCurrentUserInput = commandPoll[i + 1];
			lastTwoInputInterval = (currentUserInput.timeStamp - afterCurrentUserInput.timeStamp).GetTotalMilliseconds();
		}

		for (int32 inputQueueIndex = 0; inputQueueIndex < inputQueueDataCandidates.Num(); ++inputQueueIndex)
		{
			const auto* const inputQueueData = inputQueueDataCandidates[inputQueueIndex];
			const auto& inputActions = inputQueueData->GetInputActions();

			if (i >= inputActions.Num())
			{
				continue;
			}

			const int32 actionIndex = inputActions.Num() - i - 1;
			const FInputQueueAction& currentQueueAction = inputActions[actionIndex];

			if (currentQueueAction.command != currentUserInput.command)
			{
				// TODO: Remove elements after for-loop ends
				inputQueueDataCandidates.RemoveAt(inputQueueIndex);
				--inputQueueIndex;
				continue;
			}

			if (IsUserInputExpiredForInputQueueAction(currentUserInput, currentQueueAction))
			{
				inputQueueDataCandidates.RemoveAt(inputQueueIndex);
				--inputQueueIndex;
				continue;
			}

			const bool willPreviousTimeCheckBeDiscarded = actionIndex > 0 && 
														  commandPoll.Num() > 1 &&
														 (commandPoll[1].command == inputActions[actionIndex - 1].command);

			if (willPreviousTimeCheckBeDiscarded &&
				(currentQueueAction.minPreviousInputTime > lastTwoInputInterval ||
				currentQueueAction.maxPreviousInputTime < lastTwoInputInterval))
			{
				
				inputQueueDataCandidates.RemoveAt(inputQueueIndex);
				--inputQueueIndex;
				continue;
			}
		}

		if (inputQueueDataCandidates.IsEmpty())
		{
			return false;
		}
	}

	const UInputQueueDataAsset* const currentInputQueueData = inputQueueDataCandidates[0];

	if (inputQueueDataCandidates[0]->GetRemoveFromPollWhenInputQueueFound())
	{
		commandPoll.RemoveAt(0, currentInputQueueData->GetInputActions().Num());
	}

	UpdateDiscardInputPair(currentInputQueueData);

	LOG_TO_SCREEN("Current Command is {0}", currentInputQueueData->GetCommand());

	return commandMap->InvokeCommand(currentInputQueueData->GetCommand());
}

bool UInputQueueSystem::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
{
	if (Super::ProcessConsoleExec(Cmd, Ar, Executor))
	{
		return true;
	}

	if (commandPoll.Num() == maxPollSize - 1)
	{
		commandPoll.RemoveAt(commandPoll.Num() - 1);
	}

	commandPoll.Insert(FUserInput(Cmd), 0);

	if (ConsumeInputs())
	{
		return true;
	}

	return false;
}

void UInputQueueSystem::UpdateDiscardInputPair(const UInputQueueDataAsset* const inputQueueDataAsset)
{
	const auto& inputActions = inputQueueDataAsset->GetInputActions();

	for (const FInputQueueAction& inputAction : inputActions)
	{
		if (inputAction.bDiscardReleaseInputOfPressEvent &&
			inputAction.command[0] == '+')
		{
			FString discardCommand = inputAction.command;
			discardCommand[0] = '-';
			discardInputPairs.Add(discardCommand);
		}
	}
}

bool UInputQueueSystem::WillCurrentInputBeDiscarded(const FUserInput& userInput)
{
	for (int32 i = 0; i < discardInputPairs.Num(); ++i)
	{
		if (userInput.command[0] == '-' &&
			discardInputPairs[i] == userInput.command)
		{
			discardInputPairs.RemoveAt(i);
			return true;
		}
	}

	return false;
}

bool UInputQueueSystem::IsUserInputExpiredForInputQueueAction(const FUserInput& userInput, const FInputQueueAction& inputQueueAction)
{
	const double passedTime = (FDateTime::Now() - userInput.timeStamp).GetTotalMilliseconds();
	return passedTime > inputQueueAction.expireTime;
}
