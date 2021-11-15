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

	virtual bool IsStateSwitchable_Implementation(int state) override { return true; }

	virtual bool TryToExecuteState_Implementation(int state) override { return true; }
};
