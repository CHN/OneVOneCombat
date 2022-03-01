// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/JumpPlayerState.h"

#include "PlayerStates/MovementPlayerState.h"

#include "../MainCharacterMovementComponent.h"
#include "MainCharacter/MainCharacterData.h"
#include "MainCharacter/CharacterState.h"
#include "PlayerStateManager.h"
#include "InputQueueOutputState.h"
#include "MainCharacter.h"
#include "InputQueueSystem.h"

#include "EditorUtilities.h"

UJumpPlayerState::UJumpPlayerState()
	: Super()
{
	playerState = EPlayerState::JUMP;
}

void UJumpPlayerState::OnStateInitialized()
{
	movementComponent = mainCharacter->GetMainMovementComponent();
	characterData = mainCharacter->GetCharacterData();
	characterState = mainCharacter->GetCharacterState();

	handle = mainCharacter->GetInputQueueSystem()->BindEvent(EInputQueueOutputState::JUMP, this, &UJumpPlayerState::OnJumpActionExecuted);
}

void UJumpPlayerState::OnStateBeginPlay()
{
	movementComponent->AddVelocity(FVector::UpVector * 500.f + characterData->GetCurrentRotation() * FVector::RightVector * 400.f);

	characterState->jumpState->SetTriggerValue(true);
}

bool UJumpPlayerState::IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, EPlayerState previousState)
{
	return previousState != EPlayerState::JUMP && characterData->IsGrounded();
}

bool UJumpPlayerState::IsStateInterruptible(EPlayerState newState)
{
	return newState == EPlayerState::ATTACK;
}

void UJumpPlayerState::OnJumpActionExecuted()
{
	playerStateManager->TryToChangeCurrentState(EPlayerState::JUMP, EInputQueueOutputState::JUMP); // FIXME
}

void UJumpPlayerState::OnStateUpdate(float deltaTime)
{
	if (!characterState->jumpState->IsAnimationContinue() && characterData->IsGrounded())
	{
		EndState(EPlayerState::MOVE);
	}

	
}

void UJumpPlayerState::OnStateEndPlay()
{
	characterState->jumpState->SetTriggerValue(false);
}

void UJumpPlayerState::OnStateInterrupted()
{
	characterState->jumpState->SetTriggerValue(false);
}
