// Fill out your copyright notice in the Description page of Project Settings.


#include "InputQueueSystem.h"

#include "EditorUtilities.h"

// Sets default values for this component's properties
UInputQueueSystem::UInputQueueSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInputQueueSystem::BeginPlay()
{
	Super::BeginPlay();

	SortInputQueueDataArray();
}

void UInputQueueSystem::SortInputQueueDataArray()
{
	TArray<FInputQueueData> sortedArray;

	sortedArray.SetNum(static_cast<uint8>(EUserInputType::END_OF_ENUM));

	for (const FInputQueueData& inputQueueData : inputQueueDataArray)
	{
		if (inputQueueData.inputType < EUserInputType::END_OF_ENUM)
		{
			sortedArray[static_cast<uint8>(inputQueueData.inputType.GetValue())] = inputQueueData;
		}
	}

	inputQueueDataArray = std::move(sortedArray);
}

void UInputQueueSystem::ConsumeInputs(UPlayerInputPollingSystem* inputPollingSystem)
{
	const auto& currentInputPoll = inputPollingSystem->GetInputPoll();

	if (currentInputPoll.IsEmpty())
	{
		return;
	}

	if (WillCurrentInputBeDiscarded(currentInputPoll[0]))
	{
		inputPollingSystem->RemoveFromPolling(1);
		return;
	}

	TArray<UInputQueueDataAsset*> inputQueueDataCandidates;

	const TArray<UInputQueueDataAsset*>& selectedInputQueueDataAssets = inputQueueDataArray[static_cast<uint8>(currentInputPoll[0].inputType)].inputQueueDataAssets;

	for (UInputQueueDataAsset* const inputQueueDataAsset : selectedInputQueueDataAssets)
	{
		if (inputQueueDataAsset->GetInputActions().Num() <= currentInputPoll.Num())
		{
			inputQueueDataCandidates.Add(inputQueueDataAsset);
		}
	}

	if (inputQueueDataCandidates.IsEmpty())
	{
		return;
	}

	for (int32 i = 0; i < currentInputPoll.Num(); ++i)
	{
		const FUserInput& currentUserInput = currentInputPoll[i];

		double lastTwoInputInterval = 0.0;

		if (currentInputPoll.Num() > 1 && i < currentInputPoll.Num() - 1)
		{
			const FUserInput& afterCurrentUserInput = currentInputPoll[i + 1];
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

			if (currentQueueAction.inputType != currentUserInput.inputType ||
				currentQueueAction.inputEvent != currentUserInput.inputEvent)
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
														  currentInputPoll.Num() > 1 &&
														 (currentInputPoll[1].inputType == inputActions[actionIndex - 1].inputType &&
														  currentInputPoll[1].inputEvent == inputActions[actionIndex - 1].inputEvent);

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
			return;
		}
	}

	const UInputQueueDataAsset* const currentInputQueueData = inputQueueDataCandidates[0];

	if (inputQueueDataCandidates[0]->GetRemoveFromPollWhenInputQueueFound())
	{
		inputPollingSystem->RemoveFromPolling(currentInputQueueData->GetInputActions().Num());
	}

	UpdateDiscardInputPair(currentInputQueueData);

	LOG_TO_SCREEN("Current Command is {0}", currentInputQueueData->GetCommand());

	InvokeCommand(currentInputQueueData->GetCommand());
}

bool UInputQueueSystem::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
{
	if (Super::ProcessConsoleExec(Cmd, Ar, Executor))
	{
		return true;
	}

	FCommandData* commandData = commandDataMap.Find(Cmd);

	if (commandData && commandData->blockerCount == 0)
	{
		commandData->commandEvent.Broadcast();

		return true;
	}

	return false;
}

void UInputQueueSystem::InvokeCommand(const FString& command)
{
	auto* commandData = commandDataMap.Find(command);

	if (commandData && commandData->blockerCount == 0)
	{
		commandData->commandEvent.Broadcast();
	}
}

void UInputQueueSystem::RemoveCommand(const FString& command, const FDelegateHandle& delegateHandle)
{
	commandDataMap[command].commandEvent.Remove(delegateHandle);
}

void UInputQueueSystem::BlockCommand(const FString& cmd)
{
	FCommandData* commandData = commandDataMap.Find(cmd);

	if (commandData)
	{
		++commandData->blockerCount;
	}
}

void UInputQueueSystem::UnblockCommand(const FString& cmd)
{
	FCommandData* commandData = commandDataMap.Find(cmd);

	if (commandData && commandData->blockerCount > 0)
	{
		--commandData->blockerCount;
	}
}

void UInputQueueSystem::UpdateDiscardInputPair(const UInputQueueDataAsset* const inputQueueDataAsset)
{
	const auto& inputActions = inputQueueDataAsset->GetInputActions();

	for (const FInputQueueAction& inputAction : inputActions)
	{
		if (inputAction.bDiscardReleaseInputOfPressEvent &&
			inputAction.inputEvent == EInputEvent::IE_Pressed)
		{
			discardInputPairs.Add({ inputAction.inputType, EInputEvent::IE_Released });
		}
	}
}

bool UInputQueueSystem::WillCurrentInputBeDiscarded(const FUserInput& userInput)
{
	for (int32 i = 0; i < discardInputPairs.Num(); ++i)
	{
		if (discardInputPairs[i].inputType == userInput.inputType &&
			discardInputPairs[i].inputEvent == userInput.inputEvent)
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
