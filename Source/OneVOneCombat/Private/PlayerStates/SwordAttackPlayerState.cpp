// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/SwordAttackPlayerState.h"

#include "MainCharacter/MainCharacterData.h"
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
	return previousState == EPlayerState::MOVE || previousState == EPlayerState::JUMP;
}

void USwordAttackPlayerState::OnStateUpdate(float deltaTime)
{
	if (!mainCharacter->GetCharacterState()->swordAttackState->IsAnimationContinue())
	{
		EndState(EPlayerState::MOVE);
	}
}

void USwordAttackPlayerState::OnStateEndPlay()
{
	mainCharacter->GetCharacterState()->swordAttackState->SetTriggerValue(false);
}

void USwordAttackPlayerState::OnStateInterrupted()
{
	mainCharacter->GetCharacterState()->swordAttackState->SetTriggerValue(false);
}
