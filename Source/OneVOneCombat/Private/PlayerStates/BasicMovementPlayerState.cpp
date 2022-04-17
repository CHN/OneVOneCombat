// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicMovementPlayerState.h"

#include "PlayerStateManager.h"
#include "PlayerStateFlowManager.h"

UBasicMovementPlayerState::UBasicMovementPlayerState()
{
	playerState = EPlayerState::BASIC_MOVEMENT;
}

void UBasicMovementPlayerState::OnStateBeginPlay()
{
	movePlayerState = playerStateFlowManager->ReuseState(this, EPlayerState::MOVE);
	lookPlayerState = playerStateFlowManager->ReuseState(this, EPlayerState::LOOK);
}

void UBasicMovementPlayerState::OnStateUpdate(float deltaTime)
{
	movePlayerState->OnStateUpdate(deltaTime);
	lookPlayerState->OnStateUpdate(deltaTime);
}

bool UBasicMovementPlayerState::IsStateInterruptible(uint32 newState)
{
	return true;
}

bool UBasicMovementPlayerState::IsStateInterruptibleByInputStateOutput(EInputQueueOutputState inputOutputState, uint32 newState)
{
	return true;
}
