// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateBase.h"

#include "InputQueueOutputState.h"
#include "MainCharacter.h"
#include "PlayerStateManager.h"

UPlayerStateBase::UPlayerStateBase()
	: Super()
{
	isStatePlaying = false;
	playerState = EPlayerState::END_OF_ENUM;
}

void UPlayerStateBase::Init(TWeakObjectPtr<AMainCharacter> NewMainCharacter)
{
	mainCharacter = NewMainCharacter;
	playerStateManager = mainCharacter->GetPlayerStateManager();
	playerStateFlowManager = playerStateManager->GetStateFlowManager();
	OnStateInitialized();
}

void UPlayerStateBase::StartState_Internal()
{
	isStatePlaying = true;
	OnStateBeginPlay();
}

void UPlayerStateBase::EndState_Internal()
{
	const bool isInterrupted = IsStatePlaying();
	OnStateEndPlay(isInterrupted);
}

EPlayerState UPlayerStateBase::GetPlayerState() const
{
	return playerState;
}

void UPlayerStateBase::EndState(EPlayerState nextState)
{
	isStatePlaying = false;
	oneTimeStateEndCallback.ExecuteIfBound(nextState);
	oneTimeStateEndCallback.Unbind();
}
