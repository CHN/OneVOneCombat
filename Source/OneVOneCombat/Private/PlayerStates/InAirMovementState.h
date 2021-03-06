// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerStateBase.h"

#include "DataReadOwner.h"

#include "InAirMovementState.generated.h"

/**
 *
 */

class UMainCharacterMovementComponent;
struct FAnimationRelatedData;
struct FCharacterInputData;

UCLASS()
class UInAirMovementState : public UPlayerStateBase
{
public:

	GENERATED_BODY()

	UInAirMovementState();

	void OnStateInitialized() override;
	void OnStateUpdate(float deltaTime) override;
	bool IsStateInterruptible(uint32 newState) override;
	bool IsStateInterruptibleByCommand(const FString& command, uint32 newState) override;

private:

	TWeakObjectPtr<UMainCharacterMovementComponent> movementComponent;
	DataReadOwner<FAnimationRelatedData> animationRelatedData;
	DataReadOwner<FCharacterInputData> characterInputData;
};
