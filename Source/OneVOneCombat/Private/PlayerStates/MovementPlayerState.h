// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerStateBase.h"

#include "DataInlineSubOwner.h"

#include "MovementPlayerState.generated.h"

/**
 * 
 */

class UMainCharacterMovementComponent;

UCLASS()
class UMovementPlayerState : public UPlayerStateBase
{
public:

	GENERATED_BODY()

	UMovementPlayerState();

	void OnStateInitialized() override;
	void OnStateUpdate(float deltaTime) override;
	bool IsStateInterruptible(uint32 newState) override;
	bool IsStateInterruptibleByInputStateOutput(EInputQueueOutputState inputOutputState, uint32 newState) override;

private:

	void OnSprintKeyStateChanged(EInputEvent inputEvent);

	TWeakObjectPtr<UMainCharacterMovementComponent> movementComponent;
	DataInlineSubOwner<FAnimationRelatedData> animationRelatedData;
	DataInlineSubOwner<FCharacterStateData> characterStateData;
};
