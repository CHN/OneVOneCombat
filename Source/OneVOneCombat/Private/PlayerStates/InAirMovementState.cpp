// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/InAirMovementState.h"

#include "MainCharacterMovementComponent.h"
#include "MainCharacter/MainCharacterDataAsset.h"
#include "MainCharacter.h"
#include "PlayerStateManager.h"
#include "PlayerInputPollingSystem.h"

#include "MainCharacter/AnimationRelatedData.h"
#include "MainCharacter/CharacterStateData.h"
#include "MainCharacter/CharacterInputData.h"

UInAirMovementState::UInAirMovementState()
{
	playerState = EPlayerState::IN_AIR_MOVEMENT;
}

void UInAirMovementState::OnStateInitialized()
{
	mainCharacter->GetCharacterData()->animationRelatedDataOwner.BecomeSubOwner(&animationRelatedData);
	mainCharacter->GetCharacterData()->characterInputDataOwner.BecomeSubOwner(&characterInputData);
	movementComponent = mainCharacter->GetMainMovementComponent();
}

void UInAirMovementState::OnStateUpdate(float deltaTime)
{
	if (!movementComponent->IsMovementBeingApplied())
	{
		if (characterInputData->useRootMotion)
		{
			movementComponent->AddVelocity(animationRelatedData->rootMotionMoveDelta * deltaTime);
		}
		else
		{
			FVector moveDelta = characterInputData->rawMoveInput.GetClampedToMaxSize(1.f) * 450.f * deltaTime;
			movementComponent->AddVelocity(mainCharacter->GetActorQuat() * moveDelta);
		}
	}
}

bool UInAirMovementState::IsStateInterruptible(uint32 newState)
{
	return true;
}

bool UInAirMovementState::IsStateInterruptibleByInputStateOutput(EInputQueueOutputState inputOutputState, uint32 newState)
{
	return true;
}
