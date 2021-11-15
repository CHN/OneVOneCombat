// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//#include "PlayerMainState.h"

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
	bool IsStateSwitchable(int state);

	virtual bool IsStateSwitchable_Implementation(int state) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool TryToExecuteState(int state);

	virtual bool TryToExecuteState_Implementation(int state) = 0;
};