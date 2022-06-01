// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/JumpPlayerState.h"

#include "PlayerStates/MovementPlayerState.h"

#include "../MainCharacterMovementComponent.h"
#include "MainCharacter/MainCharacterDataAsset.h"
#include "MainCharacter/CharacterStateData.h"
#include "PlayerStateManager.h"
#include "PlayerStateFlowManager.h"
#include "InputQueueOutputState.h"
#include "CommandMap.h"
#include "MainCharacter.h"
#include "InputQueueSystem.h"
#include "CharacterEvents/CharacterEvents.h"

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
	characterData->characterStateDataOwner.BeSubOwner(&characterStateData);

	handle = mainCharacter->GetInputQueueSystem()->commandMap->BindCommand("+jumpQueue", this, &UJumpPlayerState::OnJumpActionExecuted);
}

void UJumpPlayerState::OnStateBeginPlay()
{
	FVector jumpVelocity = characterData->GetLastVelocity();
	jumpVelocity.Z = FMath::Max(jumpVelocity.Z, 500.f);
	movementComponent->AddVelocity(jumpVelocity);

	isUngrounded = false;
	groundedFrameCount = 0;
	characterStateData->isJumping = true;
	lookState = playerStateFlowManager->ReuseState(this, EPlayerState::LOOK);
	inAirMovementState = playerStateFlowManager->ReuseState(this, EPlayerState::IN_AIR_MOVEMENT);
}

bool UJumpPlayerState::IsStateTransitionInAllowedByCommand(const FString& command, uint32 previousState)
{
	return previousState != EPlayerState::JUMP && characterData->IsGrounded();
}
	
bool UJumpPlayerState::IsStateInterruptible(uint32 newState)
{
	return newState == EPlayerState::ATTACK; // FIXME: This conditions should be handled with masks
}

void UJumpPlayerState::OnJumpActionExecuted()
{
	playerStateFlowManager->TryToChangeCurrentState(EPlayerState::JUMP, "+jump"); // FIXME
}

void UJumpPlayerState::OnStateUpdate(float deltaTime)
{
	lookState->OnStateUpdate(deltaTime);
	inAirMovementState->OnStateUpdate(deltaTime);

	if (isUngrounded)
	{
		if (characterData->IsGrounded())
		{
			EndState(EPlayerState::BASIC_MOVEMENT);
		}
	}
	else if(!characterData->IsGrounded())
	{
		isUngrounded = true;
	}
	else
	{
		++groundedFrameCount;

		if (groundedFrameCount > 5)
		{
			EndState(EPlayerState::BASIC_MOVEMENT);
		}
	}
}

void UJumpPlayerState::OnStateEndPlay(bool isInterrupted, uint32 nextState)
{
	characterStateData->isJumping = false;
}