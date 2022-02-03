// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PlayerStateBase.generated.h"

UENUM()
enum class EPlayerState : uint8
{
	MOVE,
	JUMP,

	END_OF_ENUM
};

enum class EInputQueueOutputState : uint8;
class UMainCharacterData;
class UMainCharacterComponentGroup;
class UPlayerStateManager;

UCLASS(Abstract)
class UPlayerStateBase : public UActorComponent
{
public:
	
	GENERATED_BODY()

	UPlayerStateBase();

	virtual void Init(UPlayerStateManager* NewPlayerStateManager, TWeakObjectPtr<UMainCharacterData> NewCharacterData, TWeakObjectPtr<UMainCharacterComponentGroup> NewCharacterComponentGroup);

	virtual bool IsStateTransitionOutAllowed(EPlayerState newState) { return true; }
	virtual bool IsStateTransitionInAllowed(EPlayerState previousState) { return true; }

	virtual bool IsStateTransitionOutAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, EPlayerState newState) { return true; }
	virtual bool IsStateTransitionInAllowedByInputStateOutput(EInputQueueOutputState inputOutputState, EPlayerState previousState) { return true; }

	virtual void OnStateBeginPlay() {}
	virtual void OnStateUpdate(float deltaTime) {}
	virtual void OnStateEndPlay() {}
	virtual void OnStateInterrupted() {}

	void StartState_Internal();
	void EndState_Internal();

	EPlayerState GetPlayerState() const;

protected:

	EPlayerState playerState;
	TWeakObjectPtr<UMainCharacterData> characterData;
	TWeakObjectPtr<UMainCharacterComponentGroup> characterComponentGroup;
	UPlayerStateManager* playerStateManager;

	void EndState(EPlayerState nextState);

private:

	bool isStatePlaying;
};
