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
	void OnStateEndPlay() override;
	void OnStateInterrupted() override;

	bool IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, EPlayerState previousState) override;

private:
	DataInlineSubOwner<FCharacterStateData> characterStateData;
};