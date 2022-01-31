// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateManager.h"

#include "InputQueueOutputState.h"
#include "MainCharacter/MainCharacterData.h"

#include "PlayerStates/JumpPlayerState.h"

#include "EditorUtilities.h"

UPlayerStateManager::UPlayerStateManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerStateManager::Init(TObjectPtr<UMainCharacterData> characterData, UMainCharacterMovementComponent* tempMovementComp)
{
	playerStates.SetNum(static_cast<uint8>(EInputQueueOutputState::END_OF_ENUM));

	TObjectPtr<UJumpPlayerState> jumpPlayerState = NewObject<UJumpPlayerState>(this);
	jumpPlayerState->InitBase(characterData);
	jumpPlayerState->Init(tempMovementComp);
	playerStates[static_cast<uint8>(EInputQueueOutputState::JUMP)] = jumpPlayerState;
}

void UPlayerStateManager::OnInputQueueOutputStateTriggered(EInputQueueOutputState state)
{
	UPlayerStateBase* const newState = playerStates[static_cast<uint8>(state)];

	if (newState == nullptr)
	{
		return;
	}

	const bool canStateBeChanged = newState->IsStateTransitionAllowedToThisState(state);

	LOG_TO_SCREEN_STR("STATE CHANGE	: {0}, State: {1}", canStateBeChanged, EditorUtilities::EnumToString(TEXT("EInputQueueOutputState"), static_cast<uint8>(state)));

	if (canStateBeChanged)
	{
		newState->StartState_Internal();
	}
}