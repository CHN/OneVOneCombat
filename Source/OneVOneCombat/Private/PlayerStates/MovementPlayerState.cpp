// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/MovementPlayerState.h"

#include "MainCharacterMovementComponent.h"
#include "MainCharacter/MainCharacterDataAsset.h"
#include "MainCharacter.h"
#include "PlayerStateManager.h"
#include "PlayerInputPollingSystem.h"
#include "CharacterEvents/CharacterEvents.h"

#include "MainCharacter/AnimationRelatedData.h"
#include "MainCharacter/CharacterStateData.h"
#include "MainCharacter/CharacterInputData.h"

#include "MainCharacter/CharacterAttributeDataAsset.h"

UMovementPlayerState::UMovementPlayerState()
{
	playerState = EPlayerState::MOVE;
}

void UMovementPlayerState::OnStateInitialized()
{
	mainCharacter->GetCharacterData()->animationRelatedDataOwner.BeReadOwner(&animationRelatedData);
	mainCharacter->GetCharacterData()->movementComponentDataOwner.BeReadOwner(&movementComponentData);
	mainCharacter->GetCharacterData()->characterStateDataOwner.BeSubOwner(&characterStateData);
	mainCharacter->GetCharacterData()->characterInputDataOwner.BeSubOwner(&characterInputData);
	movementComponent = mainCharacter->GetMainMovementComponent();
}

void UMovementPlayerState::OnStateBeginPlay()
{
	sprintInputHandle = mainCharacter->GetPlayerInputPollingSystem()->BindInputEvent(EUserInputType::SPRINT, this, &UMovementPlayerState::OnSprintKeyStateChanged);

	sprintDisableStateOnChangeHandle = mainCharacter->GetCharacterEvents()->onSprintDisableStateChanged.AddUObject(this, &UMovementPlayerState::OnSprintDisableStateChanged);
}

void UMovementPlayerState::OnStateUpdate(float deltaTime)
{
	if (!movementComponent->IsMovementBeingApplied())
	{
		if (characterInputData->useRootMotion)
		{
			movementComponent->MoveByDelta(deltaTime, animationRelatedData->rootMotionMoveDelta * deltaTime);
		}
		else
		{
			const auto* characterAttributeData = mainCharacter->GetCharacterAttributeDataAsset();

			FVector moveDelta = characterInputData->rawMoveInput.GetClampedToMaxSize(1.f) * (characterInputData->isSprintInputInitiated ? characterAttributeData->GetSprintSpeed() : characterAttributeData->GetWalkSpeed()) * deltaTime;
			movementComponent->MoveByDelta(deltaTime, mainCharacter->GetActorQuat() * moveDelta); // FIXME: Refactor, just testing
		}

		characterStateData->isSprinting = movementComponentData->movementDelta.Size() > 10.f;
	}
}

bool UMovementPlayerState::IsStateInterruptible(uint32 newState)
{
	return true;
}

bool UMovementPlayerState::IsStateInterruptibleByInputStateOutput(EInputQueueOutputState inputOutputState, uint32 newState)
{
	return true;
}

void UMovementPlayerState::OnStateEndPlay(bool isInterrupted, uint32 nextState)
{
	characterStateData->isSprinting = false;
	mainCharacter->GetPlayerInputPollingSystem()->UnbindInputEvent(EUserInputType::SPRINT, sprintInputHandle);
	mainCharacter->GetCharacterEvents()->onSprintDisableStateChanged.Remove(sprintDisableStateOnChangeHandle);
}

void UMovementPlayerState::OnSprintKeyStateChanged(EInputEvent inputEvent)
{
	characterInputData->isSprintInputInitiated = !characterStateData->isSprintDisabled && inputEvent == EInputEvent::IE_Pressed;
	characterStateData->isSprinting = false;
}

void UMovementPlayerState::OnSprintDisableStateChanged(bool state)
{
	if (state)
	{
		isSprintInterrupted = characterInputData->isSprintInputInitiated;
		characterInputData->isSprintInputInitiated = false;
	}
	else
	{
		characterInputData->isSprintInputInitiated = isSprintInterrupted;
	}
}
