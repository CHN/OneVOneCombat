// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerStateBase.h"

#include "DataSubOwner.h"

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
	void OnStateEndPlay(bool isInterrupted, uint32 nextState) override;

	bool IsStateTransitionInAllowedByCommand(const FString& command, uint32 previousState) override;

	bool IsStateInterruptible(uint32 newState) override;

private:

	void OnJumpActionExecuted();
	void OnJumpAnimExit(const FName& /*Machine Name*/, const FName& /*State Name*/);

	bool isUngrounded;
	uint8 groundedFrameCount = 0;

	TWeakObjectPtr<UMainCharacterMovementComponent> movementComponent;
	TWeakObjectPtr<UMainCharacterDataAsset> characterData;

	TWeakObjectPtr<UPlayerStateBase> lookState;
	TWeakObjectPtr<UPlayerStateBase> inAirMovementState;

	FDelegateHandle handle;

	DataSubOwner<FCharacterStateData> characterStateData;
};