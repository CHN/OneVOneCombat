// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerStateBase.h"

#include "DataInlineSubOwner.h"

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
	void OnStateEndPlay(bool isInterrupted) override;
	void OnStateActive() override;
	void OnStateDeactive() override;

	bool IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, uint32 previousState) override;
	bool IsStateInterruptibleByInputStateOutput(EInputQueueOutputState inputOutputState, uint32 newState) override;

private:

	void OnAttackInputTriggered();

	DataInlineSubOwner<FCharacterStateData> characterStateData;
	FDelegateHandle inputEventHandle;

	TWeakObjectPtr<UPlayerStateBase> lookState;
};