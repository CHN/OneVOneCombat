// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TEnumArray.h"

#include "PlayerStateFlowManager.generated.h"

/**
 * 
 */

enum class EPlayerState : uint8;
class UPlayerStateBase;

UCLASS()
class UPlayerStateFlowManager : public UActorComponent
{
public:

	GENERATED_BODY()

	UPlayerStateFlowManager();

	bool TryToChangeCurrentState(EPlayerState nextState, EInputQueueOutputState inputReason);
	TWeakObjectPtr<UPlayerStateBase> ReusePlayerState(const UPlayerStateBase* ownerState, EPlayerState state) const;

	TWeakObjectPtr<UPlayerStateBase> DeactiveState(EPlayerState stateType);
	TWeakObjectPtr<UPlayerStateBase> ReplaceStateWith(UPlayerStateBase* playerState);
	TWeakObjectPtr<UPlayerStateBase> GetState(EPlayerState stateType) const;

private:

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnCurrentStateEndCallback(EPlayerState nextState);

	TEnumArray<UPlayerStateBase*, EPlayerState> activeStates;
	TWeakObjectPtr<UPlayerStateBase> currentState;
};
