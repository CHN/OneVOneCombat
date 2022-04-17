// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerStateBase.h"

#include "DataInlineSubOwner.h"

#include "JumpPlayerState.generated.h"

/**
 * 
 */

class UMainCharacterMovementComponent;
class UMainCharacterDataAsset;
struct FCharacterStateData;
struct FCharacterInputData;

UCLASS()
class UJumpPlayerState : public UPlayerStateBase
{
public:

	GENERATED_BODY()

	UJumpPlayerState();

	void OnStateBeginPlay() override;
	void OnStateInitialized() override;
	void OnStateUpdate(float deltaTime) override;
	void OnStateEndPlay(bool isInterrupted) override;

	bool IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, uint32 previousState) override;

	bool IsStateInterruptible(uint32 newState) override;

private:

	void OnJumpActionExecuted();
	void OnJumpAnimExit(const FName& /*Machine Name*/, const FName& /*State Name*/);

	bool isUngrounded;

	TWeakObjectPtr<UMainCharacterMovementComponent> movementComponent;
	TWeakObjectPtr<UMainCharacterDataAsset> characterData;

	TWeakObjectPtr<UPlayerStateBase> lookState;
	TWeakObjectPtr<UPlayerStateBase> inAirMovementState;

	FDelegateHandle handle;

	DataInlineSubOwner<FCharacterStateData> characterStateData;
	DataInlineSubOwner<FCharacterInputData> characterInputData;
};