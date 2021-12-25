// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateBase.h"

#include "InputQueueOutputState.h"

EInputQueueOutputState UPlayerStateBase::GetPlayerStateType() const
{
	return playerStateType;
}
