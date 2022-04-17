// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateBase.h"

#include "InputQueueOutputState.h"
#include "MainCharacter.h"
#include "PlayerStateManager.h"
#include "PlayerStateFlowManager.h"

UPlayerStateBase::UPlayerStateBase()
	: Super()
{
	isStatePlaying = false;
	playerState = uint32(-1);
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

uint32 UPlayerStateBase::GetPlayerState() const
{
	return playerState;
}

void UPlayerStateBase::EndState(uint32 nextState)
{
	isStatePlaying = false;
	oneTimeStateEndCallback.ExecuteIfBound(nextState);
	oneTimeStateEndCallback.Unbind();
}
