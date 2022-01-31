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

	void Init(TObjectPtr<UMainCharacterMovementComponent> NewMovementComponent);

	void OnStateBeginPlay() override;

	bool IsStateTransitionAllowedToThisState(EInputQueueOutputState outputState) override;

private:

	TObjectPtr<UMainCharacterMovementComponent> movementComponent;
};
