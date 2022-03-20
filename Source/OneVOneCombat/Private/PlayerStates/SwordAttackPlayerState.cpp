// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/SwordAttackPlayerState.h"

#include "MainCharacter/MainCharacterDataAsset.h"
#include "MainCharacter/CharacterStateData.h"
#include "MainCharacter.h"
#include "PlayerStateFlowManager.h"

USwordAttackPlayerState::USwordAttackPlayerState()
	: Super()
{
	playerState = EPlayerState::ATTACK;
}

void USwordAttackPlayerState::OnStateInitialized()
{
	mainCharacter->GetCharacterData()->characterStateDataOwner.BecomeSubOwner(&characterStateData);
}

void USwordAttackPlayerState::OnStateBeginPlay()
{
	characterStateData.data->isAttacking = true;
	lookState = playerStateFlowManager->ReuseState(this, EPlayerState::LOOK);
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
	if (characterStateData.data->isAttacking)
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
	lookState->OnStateUpdate(deltaTime);
	mainCharacter->GetMainMovementComponent()->MoveByDelta(deltaTime, mainCharacter->GetCharacterData()->GetCurrentRotation() * mainCharacter->GetCharacterData()->GetRawMoveInput() * 5);

	if (!characterStateData.data->isAttacking)
	{
		EndState(EPlayerState::BASIC_MOVEMENT);
	}
}

void USwordAttackPlayerState::OnStateEndPlay(bool isInterrupted)
{
	characterStateData.data->isAttacking = false;
}
