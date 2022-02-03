// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/MovementPlayerState.h"

#include "MainCharacterMovementComponent.h"
#include "MainCharacter/MainCharacterComponentGroup.h"
#include "MainCharacter/MainCharacterData.h"

UMovementPlayerState::UMovementPlayerState()
{
	playerState = EPlayerState::MOVE;
}

void UMovementPlayerState::OnStateBeginPlay()
{
	movementComponent = characterComponentGroup->GetMovementComponent();
}


void UMovementPlayerState::OnStateUpdate(float deltaTime)
{
	if (!movementComponent->IsMovementBeingApplied())
	{
		movementComponent->MoveByDelta(deltaTime, characterData->GetMovementDelta(), characterData->GetCameraRotation());
	}
}