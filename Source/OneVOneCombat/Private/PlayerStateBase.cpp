// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateBase.h"

#include "InputQueueOutputState.h"
#include "PlayerStateManager.h"

UPlayerStateBase::UPlayerStateBase()
	: Super()
{
	isStatePlaying = false;
	playerState = EPlayerState::END_OF_ENUM;
}

void UPlayerStateBase::Init(TWeakObjectPtr<UPlayerStateManager> NewPlayerStateManager, TWeakObjectPtr<AMainCharacter> NewMainCharacter)
{
	playerStateManager = NewPlayerStateManager;
	mainCharacter = NewMainCharacter;
}

void UPlayerStateBase::StartState_Internal()
{
	isStatePlaying = true;
	OnStateBeginPlay();
}

void UPlayerStateBase::EndState_Internal()
{
	if (IsStatePlaying())
	{
		isStatePlaying = false;
		OnStateInterrupted();
	}
	else
	{
		OnStateEndPlay();
	}
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
