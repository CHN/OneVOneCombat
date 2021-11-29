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
	
	inputQueueDataAssets.Sort([](UInputQueueDataAsset& prev, UInputQueueDataAsset& next)
		{
			return prev.GetPriority() <= next.GetPriority();
		});
}

void UInputQueueSystem::ConsumeInputs(UPlayerInputPollingSystem* inputPollingSystem)
{
	const auto& currentInputPoll = inputPollingSystem->GetInputPoll();

	if (currentInputPoll.IsEmpty())
	{
		return;
	}

	TArray<UInputQueueDataAsset*> inputQueueDataCandidates;

	for (const auto& inputQueueDataAsset : inputQueueDataAssets)
	{
		// TODO: Don't add to candidates if last input is not equal
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

		double lastTwoInputInterval = 0.f;
		double nextTwoInputInterval = 0.f;

		if (currentInputPoll.Num() > 1)
		{
			if (i > 0)
			{
				const FUserInput& beforeCurrentUserInput = currentInputPoll[i - 1];
				lastTwoInputInterval = (beforeCurrentUserInput.timeStamp - currentUserInput.timeStamp).GetTotalMilliseconds();
			}

			if (i < currentInputPoll.Num() - 1)
			{
				const FUserInput& afterCurrentUserInput = currentInputPoll[i + 1];
				nextTwoInputInterval = (currentUserInput.timeStamp - afterCurrentUserInput.timeStamp).GetTotalMilliseconds();
			}
		}

		LOG_TO_SCREEN_STR("lastTwoInputInterval is {0}", lastTwoInputInterval);
		LOG_TO_SCREEN_STR("nextTwoInputInterval is {0}", nextTwoInputInterval);

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

			if (currentQueueAction.minPreviousInputTime > lastTwoInputInterval ||
				currentQueueAction.maxPreviousInputTime < lastTwoInputInterval)
			{
				inputQueueDataCandidates.RemoveAt(inputQueueIndex);
				--inputQueueIndex;
				continue;
			}

			if (currentQueueAction.minNextInputTime > nextTwoInputInterval ||
				currentQueueAction.maxNextInputTime < nextTwoInputInterval)
			{
				inputQueueDataCandidates.RemoveAt(inputQueueIndex);
				--inputQueueIndex;
				continue;
			}
		}

		if (inputQueueDataCandidates.IsEmpty())
		{
			break;
		}
	}

	if (inputQueueDataCandidates.IsEmpty())
	{
		return;
	}

	inputPollingSystem->RemoveFromPolling(currentInputPoll.Num()); // TODO: Remove input from poll if expired

	LOG_TO_SCREEN_STR("Current Action is {0}", EditorUtilities::EnumToString(TEXT("EInputQueueOutputState"), static_cast<uint8>(inputQueueDataCandidates[0]->GetInputQueueOutputState())));
}