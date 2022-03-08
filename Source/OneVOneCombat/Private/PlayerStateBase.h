// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PlayerStateBase.generated.h"

enum class EInputQueueOutputState : uint8;
class AMainCharacter;
class UPlayerStateManager;
class UPlayerStateFlowManager;

DECLARE_DELEGATE_OneParam(FPlayerStateStateEndCallback, uint32);

UCLASS(Abstract, Blueprintable)
class UPlayerStateBase : public UActorComponent
{
public:
	
	GENERATED_BODY()

	UPlayerStateBase();

	void Init(TWeakObjectPtr<AMainCharacter> NewMainCharacter);

	virtual bool IsStateInterruptible(uint32 newState) { return false; }
	virtual bool IsStateTransitionInAllowed(uint32 previousState) { return true; }

	virtual bool IsStateInterruptibleByInputStateOutput(EInputQueueOutputState inputOutputState, uint32 newState) { return false; }
	virtual bool IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, uint32 previousState) { return true; }

	virtual void OnStateInitialized() {}
	virtual void OnStateReused(uint32 ownerState) {}
	virtual void OnStateBeginPlay() {}
	virtual void OnStateUpdate(float deltaTime) {}
	virtual void OnStateEndPlay(bool isInterrupted) {}
	virtual void OnStateActive() {}
	virtual void OnStateDeactive() {}

	void StartState_Internal();
	void EndState_Internal();

	uint32 GetPlayerState() const;

	inline bool IsStatePlaying() const { return isStatePlaying; }

	FPlayerStateStateEndCallback oneTimeStateEndCallback;

protected:

	uint32 playerState;
	TWeakObjectPtr<AMainCharacter> mainCharacter;
	TWeakObjectPtr<UPlayerStateManager> playerStateManager;
	TWeakObjectPtr<UPlayerStateFlowManager> playerStateFlowManager;

	void EndState(uint32 nextState);

private:

	bool isStatePlaying;
};
