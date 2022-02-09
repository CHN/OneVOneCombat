// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/MovementPlayerState.h"

#include "MainCharacterMovementComponent.h"
#include "MainCharacter/MainCharacterComponentGroup.h"
#include "MainCharacter/MainCharacterData.h"

#include "MainCharacter/AnimationRelatedData.h"

UMovementPlayerState::UMovementPlayerState()
{
	playerState = EPlayerState::MOVE;
}

void UMovementPlayerState::OnStateInitialized()
{
	characterData->animationRelatedDataOwner.BecomeSubOwner(&animationRelatedData);
}


void UMovementPlayerState::OnStateBeginPlay()
{
	movementComponent = characterComponentGroup->GetMovementComponent();
}


void UMovementPlayerState::OnStateUpdate(float deltaTime)
{
	if (!movementComponent->IsMovementBeingApplied())
	{
		movementComponent->MoveByDelta(deltaTime, animationRelatedData.data->rootMotionMoveDelta, FQuat::MakeFromEuler(FVector(0.f, 0.f, characterData->GetRawRotateInput().X)));
	}
}