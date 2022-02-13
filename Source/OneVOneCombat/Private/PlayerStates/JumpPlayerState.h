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
	void OnStateEndPlay() override;
	void OnStateInterrupted() override;

	bool IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, EPlayerState previousState) override;

	bool IsStateTransitionOutAllowed(EPlayerState newState) override;

private:
	DataInlineSubOwner<FCharacterStateData> characterStateData;

	TWeakObjectPtr<UMainCharacterMovementComponent> movementComponent;
	TWeakObjectPtr<UPlayerStateBase> swordAttackPlayerState; // FIXME: ANTIPATTERN - There should be new state with named as JumpSwordAttackPlayerState. Just testing

	bool isOneFramePassed;
};