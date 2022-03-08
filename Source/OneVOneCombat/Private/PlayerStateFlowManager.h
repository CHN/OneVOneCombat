// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TEnumArray.h"

#include "PlayerStateFlowManager.generated.h"

/**
 * 
 */

class UPlayerStateBase;

UCLASS()
class UPlayerStateFlowManager : public UObject
{
public:

	GENERATED_BODY()

	void Init(uint32 stateCount);
	void UpdateCurrentState(float deltaTime);

	bool TryToChangeCurrentState(uint32 nextState, EInputQueueOutputState inputReason);
	TWeakObjectPtr<UPlayerStateBase> ReuseState(const UPlayerStateBase* ownerState, uint32 state) const;

	TWeakObjectPtr<UPlayerStateBase> ClearState(uint32 stateType);
	TWeakObjectPtr<UPlayerStateBase> ReplaceStateWith(UPlayerStateBase* playerState);
	TWeakObjectPtr<UPlayerStateBase> GetState(uint32 stateType) const;

private:

	void OnCurrentStateEndCallback(uint32 nextState);

	TArray<UPlayerStateBase*> activeStates;
	TWeakObjectPtr<UPlayerStateBase> currentState;
};
