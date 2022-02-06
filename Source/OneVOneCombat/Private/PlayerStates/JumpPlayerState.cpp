// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/JumpPlayerState.h"

#include "PlayerStates/MovementPlayerState.h"

#include "../MainCharacterMovementComponent.h"
#include "MainCharacter/MainCharacterData.h"
#include "MainCharacter/MainCharacterComponentGroup.h"
#include "MainCharacter/MovementComponentData.h"
#include "PlayerStateManager.h"
#include "InputQueueOutputState.h"

#include "EditorUtilities.h"

UJumpPlayerState::UJumpPlayerState()
	: Super()
{
	playerState = EPlayerState::JUMP;
}

void UJumpPlayerState::OnStateInitialized()
{
	characterData->movementComponentDataOwner.BecomeSubOwner(&movementComponentData);
	movementPlayerState = playerStateManager->GetPlayerStates()[static_cast<uint32>(EPlayerState::MOVE)];
}

void UJumpPlayerState::OnStateBeginPlay()
{
	auto movementComponent = characterComponentGroup->GetMovementComponent();
	movementComponent->AddVelocity(FVector::UpVector * 300.f + characterData->GetCameraRotation() * FVector::RightVector * 400.f);

	movementComponentData.data->isJumping = true;
	isOneFramePassed = false;
}

bool UJumpPlayerState::IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, EPlayerState previousState)
{
	return previousState != EPlayerState::JUMP && characterData->IsGrounded();
}

void UJumpPlayerState::OnStateUpdate(float deltaTime)
{
	if (isOneFramePassed && characterData->IsGrounded())
	{
		movementComponentData.data->isJumping = false;
		EndState(EPlayerState::MOVE);
	}

	isOneFramePassed = true;
}