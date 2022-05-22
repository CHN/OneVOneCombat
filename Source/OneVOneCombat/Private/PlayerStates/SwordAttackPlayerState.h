// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerStateBase.h"

#include "DataSubOwner.h"

#include "SwordAttackPlayerState.generated.h"

 /**
  *
  */

struct FCharacterStateData;

UCLASS()
class USwordAttackPlayerState : public UPlayerStateBase
{
public:

	GENERATED_BODY()

	USwordAttackPlayerState();

	void OnStateBeginPlay() override;
	void OnStateInitialized() override;
	void OnStateUpdate(float deltaTime) override;
	void OnStateEndPlay(bool isInterrupted, uint32 nextState) override;
	void OnStateActive() override;
	void OnStateDeactive() override;

	bool IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, uint32 previousState) override;
	bool IsStateInterruptibleByInputStateOutput(EInputQueueOutputState inputOutputState, uint32 newState) override;

private:

	void OnAttackInputTriggered();

	DataSubOwner<FCharacterStateData> characterStateData;
	FDelegateHandle inputEventHandle;

	TWeakObjectPtr<UPlayerStateBase> basicMovementState;
};