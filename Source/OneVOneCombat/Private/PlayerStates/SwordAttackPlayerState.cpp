// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/SwordAttackPlayerState.h"

#include "MainCharacter/MainCharacterDataAsset.h"
#include "MainCharacter/CharacterStateData.h"
#include "MainCharacter.h"
#include "PlayerStateFlowManager.h"
#include "InputQueueSystem.h"
#include "PlayerStateManager.h"

USwordAttackPlayerState::USwordAttackPlayerState()
	: Super()
{
	playerState = EPlayerState::ATTACK;
}

void USwordAttackPlayerState::OnStateInitialized()
{
	mainCharacter->GetCharacterData()->characterStateDataOwner.BeSubOwner(&characterStateData);
}

void USwordAttackPlayerState::OnStateBeginPlay()
{
	characterStateData->isAttacking = true;
	basicMovementState = playerStateFlowManager->ReuseState(this, EPlayerState::BASIC_MOVEMENT);
}

void USwordAttackPlayerState::OnStateActive()
{
	inputEventHandle = mainCharacter->GetInputQueueSystem()->BindQueueEvent(EInputQueueOutputState::MELEE_ATTACK, this, &USwordAttackPlayerState::OnAttackInputTriggered);
}

void USwordAttackPlayerState::OnStateDeactive()
{
	mainCharacter->GetInputQueueSystem()->UnbindQueueEvent(EInputQueueOutputState::MELEE_ATTACK, inputEventHandle);
}

void USwordAttackPlayerState::OnAttackInputTriggered()
{
	if (characterStateData->isAttacking)
	{
		EndState(EPlayerState::BASIC_MOVEMENT);
	}
	else
	{
		playerStateFlowManager->TryToChangeCurrentState(EPlayerState::ATTACK, EInputQueueOutputState::MELEE_ATTACK);
	}
}

bool USwordAttackPlayerState::IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, uint32 previousState)
{
	return previousState == EPlayerState::BASIC_MOVEMENT;
}

bool USwordAttackPlayerState::IsStateInterruptibleByInputStateOutput(EInputQueueOutputState inputOutputState, uint32 newState)
{
	return newState == EPlayerState::JUMP;
}

void USwordAttackPlayerState::OnStateUpdate(float deltaTime)
{
	basicMovementState->OnStateUpdate(deltaTime);

	if (!characterStateData->isAttacking)
	{
		EndState(EPlayerState::BASIC_MOVEMENT);
	}
}

void USwordAttackPlayerState::OnStateEndPlay(bool isInterrupted, uint32 nextState)
{
	characterStateData->isAttacking = false;
}
