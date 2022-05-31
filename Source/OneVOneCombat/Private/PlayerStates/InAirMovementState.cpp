// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/InAirMovementState.h"

#include "MainCharacterMovementComponent.h"
#include "MainCharacter/MainCharacterDataAsset.h"
#include "MainCharacter.h"
#include "PlayerStateManager.h"

#include "MainCharacter/AnimationRelatedData.h"
#include "MainCharacter/CharacterStateData.h"
#include "MainCharacter/CharacterInputData.h"

UInAirMovementState::UInAirMovementState()
{
	playerState = EPlayerState::IN_AIR_MOVEMENT;
}

void UInAirMovementState::OnStateInitialized()
{
	mainCharacter->GetCharacterData()->animationRelatedDataOwner.BeReadOwner(&animationRelatedData);
	mainCharacter->GetCharacterData()->characterInputDataOwner.BeReadOwner(&characterInputData);
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
			FVector moveDelta = mainCharacter->GetActorQuat() * characterInputData->rawMoveInput;
			if (FVector::DotProduct(moveDelta, mainCharacter->GetCharacterData()->GetVelocity().GetUnsafeNormal()) <= 0.3f)
			{
				moveDelta = moveDelta.GetClampedToMaxSize(1.f) * 600.f * deltaTime;
				movementComponent->AddVelocity(moveDelta);
			}
		}
	}
}

bool UInAirMovementState::IsStateInterruptible(uint32 newState)
{
	return true;
}

bool UInAirMovementState::IsStateInterruptibleByCommand(const FString& command, uint32 newState)
{
	return true;
}
