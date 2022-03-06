// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/SwordAttackPlayerState.h"

#include "MainCharacter/MainCharacterDataAsset.h"
#include "MainCharacter/CharacterState.h"
#include "MainCharacter.h"

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
}

bool USwordAttackPlayerState::IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, EPlayerState previousState)
{
	return previousState == EPlayerState::MOVE;
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
	playerStateManager->TryToChangeCurrentState(EPlayerState::ATTACK, EInputQueueOutputState::MELEE_ATTACK);
}

void USwordAttackPlayerState::OnStateUpdate(float deltaTime)
{
	if (!mainCharacter->GetCharacterState()->swordAttackState->IsAnimationContinue())
	{
		EndState(EPlayerState::MOVE);
	}
}

void USwordAttackPlayerState::OnStateEndPlay(bool isInterrupted)
{
	mainCharacter->GetCharacterState()->swordAttackState->SetTriggerValue(false);
}
