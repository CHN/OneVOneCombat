// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateManager.h"

#include "InputQueueOutputState.h"
#include "MainCharacter/MainCharacterData.h"
#include "MainCharacter/MainCharacterComponentGroup.h"

#include "PlayerStates/JumpPlayerState.h"

#include "EditorUtilities.h"

UPlayerStateManager::UPlayerStateManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerStateManager::Init(TWeakObjectPtr<UMainCharacterData> characterData, TWeakObjectPtr<UMainCharacterComponentGroup> characterComponentGroup)
{
	playerStates.SetNum(static_cast<uint8>(EInputQueueOutputState::END_OF_ENUM));

	TObjectPtr<UJumpPlayerState> jumpPlayerState = NewObject<UJumpPlayerState>(this);
	jumpPlayerState->Init(characterData, characterComponentGroup);
	playerStates[static_cast<uint8>(EInputQueueOutputState::JUMP)] = jumpPlayerState;
}

void UPlayerStateManager::OnInputQueueOutputStateTriggered(EInputQueueOutputState newOutputState)
{
	bool isCurrentStateChangeable;
	EInputQueueOutputState previousOutputState;

	if (currentState.IsValid())
	{
		isCurrentStateChangeable = currentState->IsStateTransitionOutAllowed(newOutputState);
		previousOutputState = currentState->GetPlayerStateType();
	}
	else
	{
		isCurrentStateChangeable = true;
		previousOutputState = EInputQueueOutputState::END_OF_ENUM;
	}

	if (!isCurrentStateChangeable)
	{
		LOG_TO_SCREEN_STR("CURRENT STATE COULDNT BE CHANGED", 1);
		return;
	}

	auto newState = playerStates[static_cast<uint8>(newOutputState)];

	if (newState == nullptr)
	{
		LOG_TO_SCREEN_STR("NEW STATE IS NULL", 1);
		return;
	}

	const bool isNewStateChangeable = newState->IsStateTransitionInAllowed(previousOutputState);

	if (!isNewStateChangeable)
	{
		LOG_TO_SCREEN_STR("NEW STATE COULDNT BE CHANGED", 1);
		return;
	}

	LOG_TO_SCREEN_STR("STATE CHANGE: from {0} to {1}", EditorUtilities::EnumToString(TEXT("EInputQueueOutputState"), static_cast<uint8>(previousOutputState)), EditorUtilities::EnumToString(TEXT("EInputQueueOutputState"), static_cast<uint8>(newOutputState)));

	if (currentState.IsValid())
	{
		currentState->EndState_Internal();
	}

	newState->StartState_Internal();
	currentState = newState;
}