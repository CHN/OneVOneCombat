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
	lookState = playerStateFlowManager->ReusePlayerState(this, EPlayerState::LOOK);
}

bool USwordAttackPlayerState::IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, EPlayerState previousState)
{
	return previousState == EPlayerState::BASIC_MOVEMENT;
}

void USwordAttackPlayerState::OnStateActive()
{
	inputEventHandle = mainCharacter->GetInputQueueSystem()->BindEvent(EInputQueueOutputState::MELEE_ATTACK, this, &USwordAttackPlayerState::OnAttackInputTriggered);
}

void USwordAttackPlayerState::OnStateDeactive()
{
	mainCharacter->GetInputQueueSystem()->UnbindEvent(EInputQueueOutputState::MELEE_ATTACK, inputEventHandle);
}

bool USwordAttackPlayerState::IsStateInterruptibleByInputStateOutput(EInputQueueOutputState inputOutputState, EPlayerState newState)
{
	return newState == EPlayerState::JUMP;
}

void USwordAttackPlayerState::OnAttackInputTriggered()
{
	playerStateFlowManager->TryToChangeCurrentState(EPlayerState::ATTACK, EInputQueueOutputState::MELEE_ATTACK);
}

void USwordAttackPlayerState::OnStateUpdate(float deltaTime)
{
	lookState->OnStateUpdate(deltaTime);

	if (!mainCharacter->GetCharacterState()->swordAttackState->IsAnimationContinue())
	{
		EndState(EPlayerState::BASIC_MOVEMENT);
	}
}

void USwordAttackPlayerState::OnStateEndPlay(bool isInterrupted)
{
	mainCharacter->GetCharacterState()->swordAttackState->SetTriggerValue(false);
}
