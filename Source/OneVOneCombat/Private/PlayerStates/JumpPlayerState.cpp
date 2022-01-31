// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/JumpPlayerState.h"

#include "../MainCharacterMovementComponent.h"
#include "MainCharacter/MainCharacterData.h"

#include "EditorUtilities.h"

void UJumpPlayerState::Init(TObjectPtr<UMainCharacterMovementComponent> NewMovementComponent)
{
	movementComponent = NewMovementComponent;
}

void UJumpPlayerState::OnStateBeginPlay()
{
	movementComponent->AddVelocity(FVector::UpVector * 400.f);

	EndState();
}

bool UJumpPlayerState::IsStateTransitionAllowedToThisState(EInputQueueOutputState outputState)
{
	return characterData->IsGrounded();
}
