// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/MovementPlayerState.h"

#include "MainCharacterMovementComponent.h"
#include "MainCharacter/MainCharacterDataAsset.h"
#include "MainCharacter.h"

#include "MainCharacter/AnimationRelatedData.h"

UMovementPlayerState::UMovementPlayerState()
{
	playerState = EPlayerState::MOVE;
}

void UMovementPlayerState::OnStateInitialized()
{
	mainCharacter->GetCharacterData()->animationRelatedDataOwner.BecomeSubOwner(&animationRelatedData);
	movementComponent = mainCharacter->GetMainMovementComponent();
}

void UMovementPlayerState::OnStateUpdate(float deltaTime)
{
	if (!movementComponent->IsMovementBeingApplied())
	{
		movementComponent->MoveByDelta(deltaTime, animationRelatedData.data->rootMotionMoveDelta);
	}
}

bool UMovementPlayerState::IsStateInterruptible(uint32 newState)
{
	return true;
}

bool UMovementPlayerState::IsStateInterruptibleByInputStateOutput(EInputQueueOutputState inputOutputState, uint32 newState)
{
	return true;
}
