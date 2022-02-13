// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerStateBase.h"

#include "DataInlineSubOwner.h"

#include "JumpPlayerState.generated.h"

/**
 * 
 */

class UMainCharacterMovementComponent;

struct FCharacterStateData;

UCLASS()
class UJumpPlayerState : public UPlayerStateBase
{
public:

	GENERATED_BODY()

	UJumpPlayerState();

	void OnStateBeginPlay() override;
	void OnStateInitialized() override;
	void OnStateUpdate(float deltaTime) override;

	bool IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, EPlayerState previousState) override;

private:
	DataInlineSubOwner<FCharacterStateData> characterStateData;

	TWeakObjectPtr<UPlayerStateBase> movementPlayerState;
	TWeakObjectPtr<UMainCharacterMovementComponent> movementComponent;

	bool isOneFramePassed;
};