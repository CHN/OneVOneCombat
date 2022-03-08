// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateFlowManager.h"

#include "PlayerStateManager.h" // FIXME: Did I include player state manager just for an enum?
#include "PlayerStateBase.h"

UPlayerStateFlowManager::UPlayerStateFlowManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UPlayerStateFlowManager::TryToChangeCurrentState(EPlayerState nextState, EInputQueueOutputState inputReason)
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

	const EPlayerState currentPlayerState = isCurrentStateValid ? currentState->GetPlayerState() : EPlayerState::NONE;

	const bool isTransitionAllowed = newState->IsStateTransitionInAllowed(currentPlayerState);
	const bool isInputTransitionAllowed = newState->IsStateTransitionInAllowedByInputStateOutput(inputReason, currentPlayerState);

	if (!isTransitionAllowed || !isInputTransitionAllowed)
	{
		return false;
	}

	if (isCurrentStateValid)
	{
		currentState->EndState_Internal();
	}

	newState->oneTimeStateEndCallback.BindUObject(this, &UPlayerStateFlowManager::OnCurrentStateEndCallback);
	newState->StartState_Internal();
	currentState = newState;

	return true;
}

TWeakObjectPtr<UPlayerStateBase> UPlayerStateFlowManager::ReusePlayerState(const UPlayerStateBase* ownerState, EPlayerState state) const
{
	auto reusedPlayerState = activeStates[state];
	reusedPlayerState->OnStateReused(ownerState->GetPlayerState());

	return reusedPlayerState;
}

TWeakObjectPtr<UPlayerStateBase> UPlayerStateFlowManager::ClearState(EPlayerState stateType)
{
	UPlayerStateBase* previousState = activeStates[stateType];
	activeStates[stateType] = nullptr;
	return previousState;
}

TWeakObjectPtr<UPlayerStateBase> UPlayerStateFlowManager::ReplaceStateWith(UPlayerStateBase* playerState)
{
	EPlayerState stateType = playerState->GetPlayerState();
	UPlayerStateBase* previousState = activeStates[stateType];
	activeStates[stateType] = playerState;
	return previousState;
}

TWeakObjectPtr<UPlayerStateBase> UPlayerStateFlowManager::GetState(EPlayerState stateType) const
{
	return activeStates[stateType];
}

void UPlayerStateFlowManager::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	currentState->OnStateUpdate(DeltaTime);
}

void UPlayerStateFlowManager::OnCurrentStateEndCallback(EPlayerState nextState)
{
	TryToChangeCurrentState(nextState, EInputQueueOutputState::NONE);
}