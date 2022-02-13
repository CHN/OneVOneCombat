// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/SwordAttackPlayerState.h"

#include "MainCharacter/MainCharacterData.h"
#include "MainCharacter/CharacterStateData.h"

USwordAttackPlayerState::USwordAttackPlayerState()
	: Super()
{
	playerState = EPlayerState::MELEE_ATTACK;
}

void USwordAttackPlayerState::OnStateInitialized()
{
	characterData->characterStateDataOwner.BecomeSubOwner(&characterStateData);
}

void USwordAttackPlayerState::OnStateBeginPlay()
{
	characterStateData.data->isSwordAttackWanted = true;
	isOneFramePassed = false;
}

bool USwordAttackPlayerState::IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, EPlayerState previousState)
{
	return previousState == EPlayerState::MOVE || previousState == EPlayerState::JUMP;
}

void USwordAttackPlayerState::OnStateUpdate(float deltaTime)
{
	if (isOneFramePassed)
	{
		characterStateData.data->isSwordAttackWanted = false;
		EndState(EPlayerState::MOVE);
	}

	isOneFramePassed = true;
}