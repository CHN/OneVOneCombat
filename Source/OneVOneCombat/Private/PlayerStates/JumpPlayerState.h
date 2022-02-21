// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerStateBase.h"

#include "DataInlineSubOwner.h"

#include "JumpPlayerState.generated.h"

/**
 * 
 */

class UMainCharacterMovementComponent;
class UMainCharacterData;
class UCharacterState;

UCLASS()
class UJumpPlayerState : public UPlayerStateBase
{
public:

	GENERATED_BODY()

	UJumpPlayerState();

	void OnStateBeginPlay() override;
	void OnStateInitialized() override;
	void OnStateUpdate(float deltaTime) override;
	void OnStateEndPlay() override;
	void OnStateInterrupted() override;

	bool IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, EPlayerState previousState) override;

	bool IsStateInterruptible(EPlayerState newState) override;

private:

	void OnJumpActionExecuted();

	TWeakObjectPtr<UMainCharacterMovementComponent> movementComponent;
	TWeakObjectPtr<UMainCharacterData> characterData;
	TWeakObjectPtr<UCharacterState> characterState;

	FDelegateHandle handle;
};