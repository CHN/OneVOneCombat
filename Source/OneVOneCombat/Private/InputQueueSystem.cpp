// Fill out your copyright notice in the Description page of Project Settings.


#include "InputQueueSystem.h"

// Sets default values for this component's properties
UInputQueueSystem::UInputQueueSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInputQueueSystem::ConsumeInputs(UPlayerInputPollingSystem* inputPollingSystem)
{
	GEngine->AddOnScreenDebugMessage(rand(), 1.f, FColor::Blue, TEXT("XXXX"));
	const auto& currentInputPoll = inputPollingSystem->GetInputPoll();

	if (currentInputPoll.IsEmpty())
	{
		return;
	}

	for (const auto* const comboData : activeComboList)
	{
		const auto& inputSequence = comboData->GetInputSequence();

		int32 inputSequenceCount = 0;

		if (inputSequence.Num() <= currentInputPoll.Num())
		{
			for (int32 i = 0; i < inputSequence.Num(); ++i)
			{
				if (inputSequence[i] != currentInputPoll[i].inputType)
				{
					break;
				}

				++inputSequenceCount;
			}
		}

		const bool isSequenceFound = inputSequenceCount > 0;

		if (isSequenceFound)
		{
			inputPollingSystem->RemoveFromPolling(inputSequenceCount);

			GEngine->AddOnScreenDebugMessage(rand(), 1.f, FColor::Blue, FString::Printf(TEXT("Sequence Found: %d"), comboData->GetTargetPlayerState()));

			break;
		}
	}
}