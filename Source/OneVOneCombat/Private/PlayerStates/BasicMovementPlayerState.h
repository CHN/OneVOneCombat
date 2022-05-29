// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerStateBase.h"

#include "DataSubOwner.h"

#include "BasicMovementPlayerState.generated.h"

/**
 *
 */

UCLASS()
class UBasicMovementPlayerState : public UPlayerStateBase
{
public:

	GENERATED_BODY()

	UBasicMovementPlayerState();

	void OnStateBeginPlay() override;
	void OnStateUpdate(float deltaTime) override;
	bool IsStateInterruptible(uint32 newState) override;
	bool IsStateInterruptibleByCommand(const FString& command, uint32 newState) override;

private:
	TWeakObjectPtr<UPlayerStateBase> movePlayerState;
	TWeakObjectPtr<UPlayerStateBase> lookPlayerState;
};
