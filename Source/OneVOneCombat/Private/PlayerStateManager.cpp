// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateManager.h"

#include "InputQueueOutputState.h"
#include "MainCharacter/MainCharacterData.h"
#include "MainCharacter/MainCharacterComponentGroup.h"

#include "PlayerStates/JumpPlayerState.h"

#include "EditorUtilities.h"

UPlayerStateManager::UPlayerStateManager()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UPlayerStateManager::Init(TWeakObjectPtr<UMainCharacterData> characterData, TWeakObjectPtr<UMainCharacterComponentGroup> characterComponentGroup)
{
	playerStates.SetNum(static_cast<uint8>(EPlayerState::END_OF_ENUM));
	inputOutputPlayerStates.SetNum(static_cast<uint8>(EInputQueueOutputState::END_OF_ENUM));

	TObjectPtr<UJumpPlayerState> jumpPlayerState = NewObject<UJumpPlayerState>(this);
	jumpPlayerState->Init(characterData, characterComponentGroup);
	playerStates[static_cast<uint8>(EPlayerState::JUMP)] = jumpPlayerState;
	inputOutputPlayerStates[static_cast<uint8>(EInputQueueOutputState::JUMP)] = jumpPlayerState;
}

void UPlayerStateManager::OnInputQueueOutputStateTriggered(EInputQueueOutputState inputOutputState)
{
	bool isCurrentStateChangeable;

	EPlayerState newPlayerState = inputOutputPlayerStates[static_cast<uint8>(inputOutputState)]->GetPlayerState();
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

	newState->StartState_Internal();
	currentState = newState;

	LOG_TO_SCREEN("State change from {0} to {1}", EditorUtilities::EnumToString(TEXT("EPlayerState"), previousPlayerState), EditorUtilities::EnumToString(TEXT("EPlayerState"), newPlayerState));

	SetComponentTickEnabled(true);
}

void UPlayerStateManager::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	currentState->OnStateUpdate(DeltaTime);
}
