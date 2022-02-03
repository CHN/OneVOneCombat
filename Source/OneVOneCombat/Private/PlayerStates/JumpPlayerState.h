// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerStateBase.h"

#include "JumpPlayerState.generated.h"

/**
 * 
 */

class UMainCharacterMovementComponent;

UCLASS()
class UJumpPlayerState : public UPlayerStateBase
{
public:

	GENERATED_BODY()

	UJumpPlayerState();

	void OnStateBeginPlay() override;

	bool IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, EPlayerState previousState) override;

	void OnStateUpdate(float deltaTime) override;

};