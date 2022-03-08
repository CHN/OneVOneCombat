// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerStateBase.h"

#include "DataInlineSubOwner.h"

#include "LookPlayerState.generated.h"

/**
 *
 */

class UMainCharacterMovementComponent;

struct FAnimationRelatedData;

UCLASS()
class ULookPlayerState : public UPlayerStateBase
{
public:

	GENERATED_BODY()

	ULookPlayerState();

	void OnStateInitialized() override;
	void OnStateUpdate(float deltaTime) override;
	bool IsStateInterruptible(EPlayerState newState) override;
	bool IsStateInterruptibleByInputStateOutput(EInputQueueOutputState inputOutputState, EPlayerState newState) override;

private:
	TWeakObjectPtr<UMainCharacterMovementComponent> movementComponent;
};
