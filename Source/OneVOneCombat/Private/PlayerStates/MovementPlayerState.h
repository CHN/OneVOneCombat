// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerStateBase.h"

#include "DataSubOwner.h"
#include "DataReadOwner.h"

#include "MovementPlayerState.generated.h"

/**
 * 
 */

class UMainCharacterMovementComponent;
struct FAnimationRelatedData;
struct FCharacterStateData;
struct FCharacterInputData;
struct FMovementComponentData;

UCLASS()
class UMovementPlayerState : public UPlayerStateBase
{
public:

	GENERATED_BODY()

	UMovementPlayerState();

	void OnStateBeginPlay() override;
	void OnStateInitialized() override;
	void OnStateUpdate(float deltaTime) override;
	bool IsStateInterruptible(uint32 newState) override;
	bool IsStateInterruptibleByInputStateOutput(EInputQueueOutputState inputOutputState, uint32 newState) override;
	void OnStateEndPlay(bool isInterrupted, uint32 nextState) override;

private:

	void OnSprintKeyStateChanged(EInputEvent inputEvent);
	void OnSprintDisableStateChanged(bool state);

	bool isSprintInterrupted;
	FDelegateHandle sprintInputHandle;
	FDelegateHandle sprintDisableStateOnChangeHandle;

	TWeakObjectPtr<UMainCharacterMovementComponent> movementComponent;
	DataReadOwner<FAnimationRelatedData> animationRelatedData;
	DataReadOwner<FMovementComponentData> movementComponentData;
	DataSubOwner<FCharacterStateData> characterStateData;
	DataSubOwner<FCharacterInputData> characterInputData;
};
