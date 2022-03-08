// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicMovementPlayerState.h"

#include "PlayerStateFlowManager.h"

UBasicMovementPlayerState::UBasicMovementPlayerState()
{
	playerState = EPlayerState::BASIC_MOVEMENT;
}

void UBasicMovementPlayerState::OnStateBeginPlay()
{
	movePlayerState = playerStateFlowManager->ReusePlayerState(this, EPlayerState::MOVE);
	lookPlayerState = playerStateFlowManager->ReusePlayerState(this, EPlayerState::LOOK);
}

void UBasicMovementPlayerState::OnStateUpdate(float deltaTime)
{
	movePlayerState->OnStateUpdate(deltaTime);
	lookPlayerState->OnStateUpdate(deltaTime);
}

bool UBasicMovementPlayerState::IsStateInterruptible(EPlayerState newState)
{
	return true;
}

bool UBasicMovementPlayerState::IsStateInterruptibleByInputStateOutput(EInputQueueOutputState inputOutputState, EPlayerState newState)
{
	return true;
}
