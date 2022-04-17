// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/MovementPlayerState.h"

#include "MainCharacterMovementComponent.h"
#include "MainCharacter/MainCharacterDataAsset.h"
#include "MainCharacter.h"
#include "PlayerStateManager.h"
#include "PlayerInputPollingSystem.h"

#include "MainCharacter/AnimationRelatedData.h"
#include "MainCharacter/CharacterStateData.h"
#include "MainCharacter/CharacterInputData.h"

UMovementPlayerState::UMovementPlayerState()
{
	playerState = EPlayerState::MOVE;
}

void UMovementPlayerState::OnStateInitialized()
{
	mainCharacter->GetCharacterData()->animationRelatedDataOwner.BecomeSubOwner(&animationRelatedData);
	mainCharacter->GetCharacterData()->characterStateDataOwner.BecomeSubOwner(&characterStateData);
	mainCharacter->GetCharacterData()->characterInputDataOwner.BecomeSubOwner(&characterInputData);
	mainCharacter->GetCharacterData()->movementComponentDataOwner.BecomeSubOwner(&movementComponentData);
	movementComponent = mainCharacter->GetMainMovementComponent();

	mainCharacter->GetPlayerInputPollingSystem()->BindInputEvent(EUserInputType::SPRINT, this, &UMovementPlayerState::OnSprintKeyStateChanged);
}

void UMovementPlayerState::OnStateUpdate(float deltaTime)
{
	if (!movementComponent->IsMovementBeingApplied())
	{
		if (characterInputData.data->useRootMotion)
		{
			movementComponent->MoveByDelta(deltaTime, animationRelatedData.data->rootMotionMoveDelta * deltaTime);
		}
		else
		{
			FVector moveDelta = characterInputData.data->rawMoveInput.GetClampedToMaxSize(1.f) * (characterStateData.data->isSprinting ? 1000.f : 400.f) * deltaTime;
			movementComponent->MoveByDelta(deltaTime, mainCharacter->GetActorQuat() * moveDelta); // FIXME: Refactor, just testing
		}
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
