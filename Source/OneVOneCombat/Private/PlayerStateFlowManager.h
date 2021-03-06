// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TEnumArray.h"

#include "PlayerStateFlowManager.generated.h"

/**
 * 
 */

enum class EInputQueueOutputState : uint8;
class UPlayerStateBase;

UCLASS()
class UPlayerStateFlowManager : public UObject
{
public:

	GENERATED_BODY()

	void Init(uint32 stateCount, uint32 NewIdleState);
	void SetIdleState(uint32 NewIdleState);
	void UpdateCurrentState(float deltaTime);
	void SetReturnState(uint32 NewReturnState);

	bool TryToChangeCurrentState(uint32 nextState, const FString& commandReason);
	TWeakObjectPtr<UPlayerStateBase> ReuseState(const UPlayerStateBase* ownerState, uint32 state);

	TWeakObjectPtr<UPlayerStateBase> ClearState(uint32 stateType);
	TWeakObjectPtr<UPlayerStateBase> ReplaceStateWith(UPlayerStateBase* playerState);
	TWeakObjectPtr<UPlayerStateBase> GetState(uint32 stateType) const;

private:

	void OnCurrentStateEndCallback(uint32 nextState);

	int32 returnState;
	uint32 idleState;

	UPROPERTY()
	TArray<UPlayerStateBase*> activeStates;
	TWeakObjectPtr<UPlayerStateBase> currentState;

	TArray<uint32> currentReusedStates;
};
 