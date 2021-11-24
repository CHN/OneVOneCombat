// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PlayerStateLogicInterface.h"
#include "EPlayerState.h"

#include "PlayerStateObject.generated.h"

/**
 * 
 */

UCLASS(Blueprintable)
class UPlayerStateObject : public UObject, public IPlayerStateLogicInterface
{
public:

	GENERATED_BODY()

	UPlayerStateObject();
	~UPlayerStateObject();

	UFUNCTION(BlueprintCallable)
	void FireState(EPlayerState state);

	virtual bool IsStateSwitchable_Implementation(EPlayerState previousState) override { return true; }

	virtual bool TryToExecuteState_Implementation(EPlayerState previousState) override { return true; }

	virtual EPlayerState GetPlayerState_Implementation() override { return EPlayerState::FREE_STATE; }
};
 