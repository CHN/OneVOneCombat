// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateFlowManager.h"

#include "PlayerStateManager.h" // FIXME: Did I include player state manager just for an enum?
#include "PlayerStateBase.h"
#include "InputQueueOutputState.h"

void UPlayerStateFlowManager::Init(uint32 stateCount)
{
	activeStates.SetNum(stateCount);
}

bool UPlayerStateFlowManager::TryToChangeCurrentState(uint32 nextState, EInputQueueOutputState inputReason)
{
	const bool isCurrentStateValid = currentState.IsValid();
	const bool isCurrentStatePlaying = isCurrentStateValid && currentState->IsStatePlaying();

	if (isCurrentStatePlaying)
	{
		const bool isInterruptible = currentState->IsStateInterruptible(nextState);
		const bool isInputInterruptible = currentState->IsStateInterruptibleByInputStateOutput(inputReason, nextState);

		if (!isInterruptible && !isInputInterruptible)
		{
			return false;
		}
	}

	UPlayerStateBase* newState = activeStates[nextState];

	const uint32 currentPlayerState = isCurrentStateValid ? currentState->GetPlayerState() : EPlayerState::NONE;

	const bool isTransitionAllowed = newState->IsStateTransitionInAllowed(currentPlayerState);
	const bool isInputTransitionAllowed = newState->IsStateTransitionInAllowedByInputStateOutput(inputReason, currentPlayerState);

	if (!isTransitionAllowed || !isInputTransitionAllowed)
	{
		return false;
	}

	if (isCurrentStateValid)
	{
		if (!currentReusedStates.IsEmpty())
		{
			for (uint32 state : currentReusedStates)
			{
				activeStates[state]->OnStateReuseEnd(currentPlayerState);
			}
		}

		currentReusedStates.Reset();

		currentState->EndState_Internal();
	}

	newState->oneTimeStateEndCallback.BindUObject(this, &UPlayerStateFlowManager::OnCurrentStateEndCallback);
	currentState = newState;
	newState->StartState_Internal();

	return true;
}

TWeakObjectPtr<UPlayerStateBase> UPlayerStateFlowManager::ReuseState(const UPlayerStateBase* ownerState, uint32 state)
{
	auto reusedPlayerState = activeStates[state];
	reusedPlayerState->OnStateReused(ownerState->GetPlayerState());
	currentReusedStates.Add(state);

	return reusedPlayerState;
}

TWeakObjectPtr<UPlayerStateBase> UPlayerStateFlowManager::ClearState(uint32 stateType)
{
	UPlayerStateBase* previousState = activeStates[stateType];
	activeStates[stateType] = nullptr;
	return previousState;
}

TWeakObjectPtr<UPlayerStateBase> UPlayerStateFlowManager::ReplaceStateWith(UPlayerStateBase* playerState)
{
	uint32 stateType = playerState->GetPlayerState();
	UPlayerStateBase* previousState = activeStates[stateType];
	activeStates[stateType] = playerState;
	return previousState;
}

TWeakObjectPtr<UPlayerStateBase> UPlayerStateFlowManager::GetState(uint32 stateType) const
{
	return activeStates[stateType];
}

void UPlayerStateFlowManager::UpdateCurrentState(float deltaTime)
{
	currentState->OnStateUpdate(deltaTime);
}

void UPlayerStateFlowManager::OnCurrentStateEndCallback(uint32 nextState)
{
	TryToChangeCurrentState(nextState, EInputQueueOutputState::NONE);
}