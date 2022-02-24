// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateManager.h"

#include "InputQueueOutputState.h"
#include "MainCharacter/MainCharacterData.h"
#include "MainCharacter/CharacterState.h"

#include "PlayerStates/JumpPlayerState.h"
#include "PlayerStates/MovementPlayerState.h"
#include "PlayerStates/SwordAttackPlayerState.h"

#include "EditorUtilities.h"

UPlayerStateManager::UPlayerStateManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

template<typename T>
void UPlayerStateManager::CreatePlayerState(EPlayerState playerState)
{
	TObjectPtr<T> state = NewObject<T>(this);
	state->Init(this, mainCharacter);
	playerStates[static_cast<uint8>(playerState)] = state;
}

void UPlayerStateManager::Init(TWeakObjectPtr<AMainCharacter> NewMainCharacter)
{
	mainCharacter = NewMainCharacter;

	playerStates.SetNum(static_cast<uint8>(EPlayerState::END_OF_ENUM));

	CreatePlayerState<UMovementPlayerState>(EPlayerState::MOVE);
	CreatePlayerState<UJumpPlayerState>(EPlayerState::JUMP);
	CreatePlayerState<USwordAttackPlayerState>(EPlayerState::MELEE_ATTACK);

	for (auto playerState : playerStates)
	{
		playerState->OnStateInitialized();
	}

	TryToChangeCurrentState(EPlayerState::MOVE, EInputQueueOutputState::NONE);
}

bool UPlayerStateManager::TryToChangeCurrentState(EPlayerState nextState, EInputQueueOutputState inputReason)
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

	auto newState = playerStates[static_cast<uint8>(nextState)];

	const EPlayerState currentPlayerState = isCurrentStateValid ? currentState->GetPlayerState() : EPlayerState::NONE;

	const bool isInterruptible = newState->IsStateTransitionInAllowed(currentPlayerState);
	const bool isInputInterruptible = newState->IsStateTransitionInAllowedByInputStateOutput(inputReason, currentPlayerState);

	if (!isInterruptible && !isInputInterruptible)
	{
		return false;
	}

	if (isCurrentStateValid)
	{
		currentState->EndState_Internal();
	}

	newState->oneTimeStateEndCallback.BindUObject(this, &UPlayerStateManager::OnCurrentStateEndCallback);
	newState->StartState_Internal();
	currentState = newState;

	return true;
}

void UPlayerStateManager::OnCurrentStateEndCallback(EPlayerState nextState)
{
	TryToChangeCurrentState(nextState, EInputQueueOutputState::NONE);
}

const TArray<TWeakObjectPtr<UPlayerStateBase>>& UPlayerStateManager::GetPlayerStates() const
{
	return playerStates;
}

void UPlayerStateManager::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	currentState->OnStateUpdate(DeltaTime);
}
