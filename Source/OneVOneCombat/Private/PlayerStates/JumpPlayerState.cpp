// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/JumpPlayerState.h"

#include "PlayerStates/MovementPlayerState.h"

#include "../MainCharacterMovementComponent.h"
#include "MainCharacter/MainCharacterData.h"
#include "MainCharacter/MainCharacterComponentGroup.h"
#include "MainCharacter/CharacterStateData.h"
#include "PlayerStateManager.h"
#include "InputQueueOutputState.h"

#include "SwordAttackPlayerState.h" // FIXME: ANTIPATTERN - There should be new state with named as JumpSwordAttackPlayerState. Just testing

#include "EditorUtilities.h"

UJumpPlayerState::UJumpPlayerState()
	: Super()
{
	playerState = EPlayerState::JUMP;
}

void UJumpPlayerState::OnStateInitialized()
{
	characterData->characterStateDataOwner.BecomeSubOwner(&characterStateData);
	movementComponent = characterComponentGroup->GetMovementComponent();
	swordAttackPlayerState = playerStateManager->GetPlayerStates()[static_cast<uint32>(EPlayerState::MELEE_ATTACK)];
}

void UJumpPlayerState::OnStateBeginPlay()
{
	movementComponent->AddVelocity(FVector::UpVector * 500.f + characterData->GetCurrentRotation() * FVector::RightVector * 400.f);

	characterStateData.data->isJumping = true;
	isOneFramePassed = false;
}

bool UJumpPlayerState::IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, EPlayerState previousState)
{
	return previousState != EPlayerState::JUMP && characterData->IsGrounded();
}

bool UJumpPlayerState::IsStateTransitionOutAllowed(EPlayerState newState)
{
	return newState == EPlayerState::MELEE_ATTACK;
}

void UJumpPlayerState::OnStateUpdate(float deltaTime)
{
	movementComponent->MoveByDelta(deltaTime, characterData->GetCurrentRotation() * FVector(characterData->GetRawMoveInput().X * -3.f, FMath::Min(characterData->GetRawMoveInput().Y * 6.f, 0.f), 0.f), FQuat::MakeFromEuler(FVector(0.f, 0.f, characterData->GetRawRotateInput().X))); // FIXME: I am sleepy, so testing code was added directly

	if (isOneFramePassed && characterData->IsGrounded())
	{
		characterStateData.data->isJumping = false;
		EndState(EPlayerState::MOVE);
	}

	isOneFramePassed = true;
}

void UJumpPlayerState::OnStateEndPlay()
{
	characterStateData.data->isJumping = false;
}

void UJumpPlayerState::OnStateInterrupted()
{
	characterStateData.data->isJumping = false;
}
