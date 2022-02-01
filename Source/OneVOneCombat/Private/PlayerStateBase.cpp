// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateBase.h"

#include "InputQueueOutputState.h"

UPlayerStateBase::UPlayerStateBase()
	: Super()
{
	isStatePlaying = false;
	playerStateType = EInputQueueOutputState::END_OF_ENUM;
}

void UPlayerStateBase::Init(TWeakObjectPtr<UMainCharacterData> NewCharacterData, TWeakObjectPtr<UMainCharacterComponentGroup> NewCharacterComponentGroup)
{
	characterData = NewCharacterData;
	characterComponentGroup = NewCharacterComponentGroup;
}

void UPlayerStateBase::StartState_Internal()
{
	isStatePlaying = true;
	OnStateBeginPlay();
}

void UPlayerStateBase::EndState_Internal()
{
	if (isStatePlaying)
	{
		OnStateInterrupted();
	}
	else
	{
		OnStateEndPlay();
	}

	isStatePlaying = false;
}

EInputQueueOutputState UPlayerStateBase::GetPlayerStateType() const
{
	return playerStateType;
}

void UPlayerStateBase::EndState()
{
	isStatePlaying = false;
	OnStateEndPlay();
}
