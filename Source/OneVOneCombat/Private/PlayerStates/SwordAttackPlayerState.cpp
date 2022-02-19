// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/SwordAttackPlayerState.h"

#include "MainCharacter/MainCharacterData.h"
#include "MainCharacter/CharacterState.h"

USwordAttackPlayerState::USwordAttackPlayerState()
	: Super()
{
	playerState = EPlayerState::MELEE_ATTACK;
}

void USwordAttackPlayerState::OnStateInitialized()
{

}

void USwordAttackPlayerState::OnStateBeginPlay()
{
	characterState->swordAttackState->SetTriggerValue(true);
}

bool USwordAttackPlayerState::IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, EPlayerState previousState)
{
	return previousState == EPlayerState::MOVE || previousState == EPlayerState::JUMP;
}

void USwordAttackPlayerState::OnStateUpdate(float deltaTime)
{
	if (!characterState->swordAttackState->IsAnimationContinue())
	{
		EndState(EPlayerState::MOVE);
	}
}

void USwordAttackPlayerState::OnStateEndPlay()
{
	characterState->swordAttackState->SetTriggerValue(false);
}

void USwordAttackPlayerState::OnStateInterrupted()
{
	characterState->swordAttackState->SetTriggerValue(false);
}
