// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "EPlayerState.h"

#include "PlayerStateLogicInterface.generated.h"

/**
 * 
 */

UINTERFACE(MinimalAPI, Blueprintable)
class UPlayerStateLogicInterface : public UInterface
{
	GENERATED_BODY()

public:

	UPlayerStateLogicInterface();
	~UPlayerStateLogicInterface();
};

class IPlayerStateLogicInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsStateSwitchable(EPlayerState previousState);

	virtual bool IsStateSwitchable_Implementation(EPlayerState previousState) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool TryToExecuteState(EPlayerState previousState);

	virtual bool TryToExecuteState_Implementation(EPlayerState previousState) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EPlayerState GetPlayerState();

	virtual EPlayerState GetPlayerState_Implementation() = 0;
};