// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateManager.h"

#include "InputQueueOutputState.h"
#include "MainCharacter/MainCharacterData.h"
#include "MainCharacter/MainCharacterComponentGroup.h"

#include "PlayerStates/JumpPlayerState.h"
#include "PlayerStates/MovementPlayerState.h"

#include "EditorUtilities.h"

UPlayerStateManager::UPlayerStateManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerStateManager::Init(TWeakObjectPtr<UMainCharacterData> characterData, TWeakObjectPtr<UMainCharacterComponentGroup> characterComponentGroup)
{
	playerStates.SetNum(static_cast<uint8>(EPlayerState::END_OF_ENUM));
	inputOutputPlayerStates.SetNum(static_cast<uint8>(EInputQueueOutputState::END_OF_ENUM));

	TObjectPtr<UJumpPlayerState> jumpPlayerState = NewObject<UJumpPlayerState>(this);
	jumpPlayerState->Init(this, characterData, characterComponentGroup);
	playerStates[static_cast<uint8>(EPlayerState::JUMP)] = jumpPlayerState;
	inputOutputPlayerStates[static_cast<uint8>(EInputQueueOutputState::JUMP)] = jumpPlayerState;

	TObjectPtr<UMovementPlayerState> movementPlayerState = NewObject<UMovementPlayerState>(this);
	movementPlayerState->Init(this, characterData, characterComponentGroup);
	playerStates[static_cast<uint8>(EPlayerState::MOVE)] = movementPlayerState;

	for (auto playerState : playerStates)
	{
		playerState->OnStateInitialized();
	}

	currentState = movementPlayerState;
	currentState->StartState_Internal();
}

void UPlayerStateManager::OnInputQueueOutputStateTriggered(EInputQueueOutputState inputOutputState)
{
	bool isCurrentStateChangeable;

	EPlayerState newPlayerState;

	if (!inputOutputPlayerStates[static_cast<uint8>(inputOutputState)].IsValid())
	{
		return;
	}

	newPlayerState = inputOutputPlayerStates[static_cast<uint8>(inputOutputState)]->GetPlayerState();

	EPlayerState previousPlayerState;

	if (currentState.IsValid())
	{
		isCurrentStateChangeable = currentState->IsStateTransitionOutAllowedByInputStateOutput(inputOutputState, newPlayerState);
		previousPlayerState = currentState->GetPlayerState();
	}
	else
	{
		isCurrentStateChangeable = true;
		previousPlayerState = EPlayerState::END_OF_ENUM;
	}

	if (!isCurrentStateChangeable)
	{
		LOG_TO_SCREEN("CURRENT STATE COULDNT BE CHANGED");
		return;
	}

	auto newState = inputOutputPlayerStates[static_cast<uint8>(inputOutputState)];

	if (newState == nullptr)
	{
		LOG_TO_SCREEN("NEW STATE IS NULL");
		return;
	}

	const bool isNewStateChangeable = newState->IsStateTransitionInAllowedByInputStateOutput(inputOutputState, previousPlayerState);

	if (!isNewStateChangeable)
	{
		LOG_TO_SCREEN("NEW STATE COULDNT BE CHANGED");
		return;
	}

	if (currentState.IsValid())
	{
		currentState->EndState_Internal();
	}

	currentState = newState;
	currentState->StartState_Internal();

	LOG_TO_SCREEN("State change from {0} to {1}", EditorUtilities::EnumToString(TEXT("EPlayerState"), previousPlayerState), EditorUtilities::EnumToString(TEXT("EPlayerState"), newPlayerState));
}

void UPlayerStateManager::TryToChangeNextState(EPlayerState nextState)
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
