// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerStateBase.h"

#include "MovementPlayerState.generated.h"

/**
 * 
 */

class UMainCharacterMovementComponent;

UCLASS()
class UMovementPlayerState : public UPlayerStateBase
{
public:

	GENERATED_BODY()

	UMovementPlayerState();

	void OnStateBeginPlay() override;
	void OnStateUpdate(float deltaTime) override;

private:
	TWeakObjectPtr<UMainCharacterMovementComponent> movementComponent;
};
