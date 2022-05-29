// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PlayerStateBase.generated.h"

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

	void Init(AMainCharacter* NewMainCharacter);

	virtual bool IsStateInterruptible(uint32 newState) { return false; }
	virtual bool IsStateTransitionInAllowed(uint32 previousState) { return true; }

	virtual bool IsStateInterruptibleByCommand(const FString& command, uint32 newState) { return false; }
	virtual bool IsStateTransitionInAllowedByCommand(const FString& command, uint32 previousState) { return true; }

	virtual void OnStateInitialized() {}
	virtual void OnStateReused(uint32 ownerState) { OnStateBeginPlay(); }
	virtual void OnStateBeginPlay() {}
	virtual void OnStateUpdate(float deltaTime) {}
	virtual void OnStateEndPlay(bool isInterrupted, uint32 nextState) {}
	virtual void OnStateReuseEnd(uint32 ownerState, uint32 nextState) { EndState_Internal(nextState); }
	virtual void OnStateActive() {}
	virtual void OnStateDeactive() {}

	void StartState_Internal();
	void EndState_Internal(uint32 nextState);

	uint32 GetPlayerState() const;

	inline bool IsStatePlaying() const { return isStatePlaying; }

	FPlayerStateStateEndCallback oneTimeStateEndCallback;

protected:

	uint32 playerState;

	UPROPERTY()
	AMainCharacter* mainCharacter;

	UPROPERTY()
	UPlayerStateManager* playerStateManager;

	UPROPERTY()
	UPlayerStateFlowManager* playerStateFlowManager;

	void EndState(uint32 nextState);

private:

	bool isStatePlaying;
};
