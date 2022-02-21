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

	currentState = playerStates[static_cast<uint8>(EPlayerState::MOVE)];
	currentState->StartState_Internal();
}

bool UPlayerStateManager::TryToChangeCurrentState(EPlayerState nextState, EInputQueueOutputState inputReason)
{
	if (currentState.IsValid())
	{
		bool isCurrentStateChangeable = currentState->IsStateInterruptibleByInputStateOutput(inputReason, nextState);

		auto newState = playerStates[static_cast<uint8>(nextState)];

		bool isNextStateChangeable = newState->IsStateTransitionInAllowedByInputStateOutput(inputReason, currentState->GetPlayerState());

		bool isStateChangeable = isCurrentStateChangeable && isNextStateChangeable;

		if (isStateChangeable)
		{
			currentState->EndState_Internal();
			newState->StartState_Internal();
			currentState = newState;
		}

		return isStateChangeable;
	}
	
	return false;
}

void UPlayerStateManager::ChangeNextStateOnStateEnd(EPlayerState nextState)
{
	EPlayerState previousPlayerState = currentState->GetPlayerState();

	auto newState = playerStates[static_cast<uint8>(nextState)];

	if (newState == nullptr)
	{
		LOG_TO_SCREEN("NEW STATE IS NULL");
		return;
	}

	const bool isNewStateChangeable = newState->IsStateTransitionInAllowed(previousPlayerState);

	if (!isNewStateChangeable)
	{
		LOG_TO_SCREEN("NEW STATE COULDNT BE CHANGED");
		return;
	}

	newState->StartState_Internal();
	currentState = newState;

	LOG_TO_SCREEN("State change from {0} to {1}", EditorUtilities::EnumToString(TEXT("EPlayerState"), previousPlayerState), EditorUtilities::EnumToString(TEXT("EPlayerState"), nextState));
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
