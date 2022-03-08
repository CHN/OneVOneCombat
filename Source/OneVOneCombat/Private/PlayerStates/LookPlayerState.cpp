// Fill out your copyright notice in the Description page of Project Settings.


#include "LookPlayerState.h"

#include "MainCharacterMovementComponent.h"
#include "MainCharacter/MainCharacterDataAsset.h"
#include "MainCharacter.h"

ULookPlayerState::ULookPlayerState()
{
	playerState = EPlayerState::LOOK;
}

void ULookPlayerState::OnStateInitialized()
{
	movementComponent = mainCharacter->GetMainMovementComponent();
}

void ULookPlayerState::OnStateUpdate(float deltaTime)
{
	movementComponent->RotateByDelta(FQuat::MakeFromEuler(FVector(0.f, 0.f, mainCharacter->GetCharacterData()->GetRawRotateInput().X)));
}

bool ULookPlayerState::IsStateInterruptible(uint32 newState)
{
	return true;
}

bool ULookPlayerState::IsStateInterruptibleByInputStateOutput(EInputQueueOutputState inputOutputState, uint32 newState)
{
	return true;
}
