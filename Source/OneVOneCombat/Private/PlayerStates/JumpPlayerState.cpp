// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/JumpPlayerState.h"

#include "PlayerStates/MovementPlayerState.h"

#include "../MainCharacterMovementComponent.h"
#include "MainCharacter/MainCharacterDataAsset.h"
#include "MainCharacter/CharacterStateData.h"
#include "PlayerStateManager.h"
#include "PlayerStateFlowManager.h"
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
	characterData->characterStateDataOwner.BecomeSubOwner(&characterStateData);

	handle = mainCharacter->GetInputQueueSystem()->BindEvent(EInputQueueOutputState::JUMP, this, &UJumpPlayerState::OnJumpActionExecuted);

	mainCharacter->GetAnimInstance()->AddNativeStateExitBinding(FName("DefaultMachine"), FName("Jumping"), FOnGraphStateChanged::CreateUObject(this, &UJumpPlayerState::OnJumpAnimExit));
}

void UJumpPlayerState::OnStateBeginPlay()
{
	movementComponent->AddVelocity(FVector::UpVector * 500.f + characterData->GetCurrentRotation() * FVector::RightVector * 400.f);

	characterStateData.data->isJumping = true;
	characterStateData.data->isJumpingAnimationActive = true;
	lookState = playerStateFlowManager->ReuseState(this, EPlayerState::LOOK);
}

bool UJumpPlayerState::IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, uint32 previousState)
{
	return previousState != EPlayerState::JUMP && characterData->IsGrounded();
}

bool UJumpPlayerState::IsStateInterruptible(uint32 newState)
{
	return newState == EPlayerState::ATTACK; // FIXME: This conditions should be handled with masks
}

void UJumpPlayerState::OnJumpActionExecuted()
{
	playerStateFlowManager->TryToChangeCurrentState(EPlayerState::JUMP, EInputQueueOutputState::JUMP); // FIXME
}

void UJumpPlayerState::OnJumpAnimExit(const struct FAnimNode_StateMachine& /*Machine*/, int32 /*PrevStateIndex*/, int32 /*NextStateIndex*/)
{
	characterStateData.data->isJumping = false;
}

void UJumpPlayerState::OnStateUpdate(float deltaTime)
{
	lookState->OnStateUpdate(deltaTime);

	if (!characterStateData.data->isJumping && characterData->IsGrounded())
	{
		EndState(EPlayerState::BASIC_MOVEMENT);
	}
}

void UJumpPlayerState::OnStateEndPlay(bool isInterrupted)
{
	characterStateData.data->isJumping = false;
}