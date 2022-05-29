// Fill out your copyright notice in the Description page of Project Settings.


#include "LookPlayerState.h"

#include "MainCharacterPlayerState.h"
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
	mainCharacter->GetCharacterData()->characterInputDataOwner.BeReadOwner(&inputData);
}

void ULookPlayerState::OnStateUpdate(float deltaTime)
{
	movementComponent->RotateByDelta(FQuat(FVector::UpVector, mainCharacter->GetPlayerState()->rawLookInput.X));
	movementComponent->RotateVerticalRotationComponent(FQuat(FVector::ForwardVector, mainCharacter->GetPlayerState()->rawLookInput.Y));
}

bool ULookPlayerState::IsStateInterruptible(uint32 newState)
{
	return true;
}

bool ULookPlayerState::IsStateInterruptibleByCommand(const FString& command, uint32 newState)
{
	return true;
}
