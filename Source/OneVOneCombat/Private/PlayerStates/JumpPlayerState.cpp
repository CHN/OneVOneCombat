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
	playerStateType = EInputQueueOutputState::JUMP;
}

void UJumpPlayerState::OnStateBeginPlay()
{
	auto movementComponent = characterComponentGroup->GetMovementComponent();
	movementComponent->AddVelocity(FVector::UpVector * 400.f);

	EndState(); // TODO: EndState method should not be at here, can state machine be stateless? LoL, just do a transition to the default state
}

bool UJumpPlayerState::IsStateTransitionInAllowed(EInputQueueOutputState previousState)
{
	return characterData->IsGrounded();
}