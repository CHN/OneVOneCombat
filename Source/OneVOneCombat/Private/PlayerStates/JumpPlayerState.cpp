// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/JumpPlayerState.h"

#include "../MainCharacterMovementComponent.h"
#include "MainCharacter/MainCharacterData.h"
#include "MainCharacter/MainCharacterComponentGroup.h"
#include "InputQueueOutputState.h"

#include "EditorUtilities.h"

UJumpPlayerState::UJumpPlayerState()
	: Super()
{
	playerState = EPlayerState::JUMP;
}

void UJumpPlayerState::OnStateBeginPlay()
{
	auto movementComponent = characterComponentGroup->GetMovementComponent();
	movementComponent->AddVelocity(FVector::UpVector * 400.f);

	EndState(EPlayerState::MOVE);
}

bool UJumpPlayerState::IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, EPlayerState previousState)
{
	return characterData->IsGrounded();
}

void UJumpPlayerState::OnStateUpdate(float deltaTime)
{
	
}
