// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateBase.h"

#include "InputQueueOutputState.h"

UPlayerStateBase::UPlayerStateBase()
	: Super()
{
	isStatePlaying = false;
}

void UPlayerStateBase::InitBase(TObjectPtr<UMainCharacterData> NewCharacterData)
{
	characterData = NewCharacterData;
}

void UPlayerStateBase::StartState_Internal()
{
	isStatePlaying = true;
	OnStateBeginPlay();
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
