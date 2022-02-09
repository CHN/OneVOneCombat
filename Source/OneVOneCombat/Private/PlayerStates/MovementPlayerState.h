// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerStateBase.h"

#include "DataInlineSubOwner.h"

#include "MovementPlayerState.generated.h"

/**
 * 
 */

class UMainCharacterMovementComponent;

struct FAnimationRelatedData;

UCLASS()
class UMovementPlayerState : public UPlayerStateBase
{
public:

	GENERATED_BODY()

	UMovementPlayerState();

	void OnStateInitialized() override;
	void OnStateBeginPlay() override;
	void OnStateUpdate(float deltaTime) override;

private:
	TWeakObjectPtr<UMainCharacterMovementComponent> movementComponent;
	DataInlineSubOwner<FAnimationRelatedData> animationRelatedData;
};
