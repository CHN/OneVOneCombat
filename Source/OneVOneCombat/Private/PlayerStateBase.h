// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PlayerStateBase.generated.h"

enum class EInputQueueOutputState : uint8;
class AMainCharacter;
class UPlayerStateManager;

DECLARE_DELEGATE_OneParam(FPlayerStateStateEndCallback, EPlayerState);

UCLASS(Abstract)
class UPlayerStateBase : public UActorComponent
{
public:
	
	GENERATED_BODY()

	UPlayerStateBase();

	void Init(TWeakObjectPtr<AMainCharacter> NewMainCharacter);

	virtual bool IsStateInterruptible(EPlayerState newState) { return false; }
	virtual bool IsStateTransitionInAllowed(EPlayerState previousState) { return true; }

	virtual bool IsStateInterruptibleByInputStateOutput(EInputQueueOutputState inputOutputState, EPlayerState newState) { return false; }
	virtual bool IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, EPlayerState previousState) { return true; }

	virtual void OnStateInitialized() {}
	virtual void OnStateReused(EPlayerState ownerState) {}
	virtual void OnStateBeginPlay() {}
	virtual void OnStateUpdate(float deltaTime) {}
	virtual void OnStateEndPlay() {}
	virtual void OnStateInterrupted() {}
	virtual void OnStateActive() {}
	virtual void OnStateDeactive() {}

	void StartState_Internal();
	void EndState_Internal();

	EPlayerState GetPlayerState() const;

	inline bool IsStatePlaying() const { return isStatePlaying; }

	FPlayerStateStateEndCallback oneTimeStateEndCallback;

protected:

	EPlayerState playerState;
	TWeakObjectPtr<AMainCharacter> mainCharacter;
	TWeakObjectPtr<UPlayerStateManager> playerStateManager;

	void EndState(EPlayerState nextState);

private:

	bool isStatePlaying;
};
