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
	mainCharacter->GetCharacterData()->characterStateDataOwner.BecomeSubOwner(&characterStateData);
	movementComponent = mainCharacter->GetMainMovementComponent();

	mainCharacter->GetPlayerInputPollingSystem()->BindInputEvent(EUserInputType::SPRINT, this, &UMovementPlayerState::OnSprintKeyStateChanged);
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

void UMovementPlayerState::OnSprintKeyStateChanged(EInputEvent inputEvent)
{
	characterStateData.data->isSprinting = inputEvent == EInputEvent::IE_Pressed;
}
