// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/SwordAttackPlayerState.h"

#include "MainCharacter/MainCharacterDataAsset.h"
#include "MainCharacter/CharacterState.h"
#include "MainCharacter.h"
#include "PlayerStateFlowManager.h"

USwordAttackPlayerState::USwordAttackPlayerState()
	: Super()
{
	playerState = EPlayerState::ATTACK;
}

void USwordAttackPlayerState::OnStateInitialized()
{

}

void USwordAttackPlayerState::OnStateBeginPlay()
{
	mainCharacter->GetCharacterState()->swordAttackState->SetTriggerValue(true);
	lookState = playerStateFlowManager->ReuseState(this, EPlayerState::LOOK);
}

void USwordAttackPlayerState::OnStateActive()
{
	inputEventHandle = mainCharacter->GetInputQueueSystem()->BindEvent(EInputQueueOutputState::MELEE_ATTACK, this, &USwordAttackPlayerState::OnAttackInputTriggered);
}

void USwordAttackPlayerState::OnStateDeactive()
{
	mainCharacter->GetInputQueueSystem()->UnbindEvent(EInputQueueOutputState::MELEE_ATTACK, inputEventHandle);
}

void USwordAttackPlayerState::OnAttackInputTriggered()
{
	playerStateFlowManager->TryToChangeCurrentState(EPlayerState::ATTACK, EInputQueueOutputState::MELEE_ATTACK);
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

	if (!mainCharacter->GetCharacterState()->swordAttackState->IsAnimationContinue())
	{
		EndState(EPlayerState::BASIC_MOVEMENT);
	}
}

void USwordAttackPlayerState::OnStateEndPlay(bool isInterrupted)
{
	mainCharacter->GetCharacterState()->swordAttackState->SetTriggerValue(false);
}
