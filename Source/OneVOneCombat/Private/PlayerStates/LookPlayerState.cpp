// Fill out your copyright notice in the Description page of Project Settings.


#include "LookPlayerState.h"

#include "MainCharacterMovementComponent.h"
#include "MainCharacter/MainCharacterDataAsset.h"
#include "MainCharacter.h"
#include "PlayerStateManager.h"
#include "MainCharacter/CharacterInputData.h"

ULookPlayerState::ULookPlayerState()
{
	playerState = EPlayerState::LOOK;
}

void ULookPlayerState::OnStateInitialized()
{
	movementComponent = mainCharacter->GetMainMovementComponent();
	mainCharacter->GetCharacterData()->characterInputDataOwner.BecomeSubOwner(&inputData);
}

void ULookPlayerState::OnStateUpdate(float deltaTime)
{
	movementComponent->RotateByDelta(FQuat(FVector::UpVector, inputData->scaledRotateInput.X));
	movementComponent->RotateVerticalRotationComponent(FQuat(FVector::ForwardVector, inputData->scaledRotateInput.Y));
}

bool ULookPlayerState::IsStateInterruptible(uint32 newState)
{
	return true;
}

bool ULookPlayerState::IsStateInterruptibleByInputStateOutput(EInputQueueOutputState inputOutputState, uint32 newState)
{
	return true;
}
