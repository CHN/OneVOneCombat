// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PlayerStateBase.generated.h"

enum class EInputQueueOutputState : uint8;
class UMainCharacterData;
class UMainCharacterComponentGroup;

UCLASS(Abstract)
class UPlayerStateBase : public UActorComponent
{
public:
	
	GENERATED_BODY()

	UPlayerStateBase();

	virtual void Init(TWeakObjectPtr<UMainCharacterData> NewCharacterData, TWeakObjectPtr<UMainCharacterComponentGroup> NewCharacterComponentGroup);

	virtual bool IsStateTransitionOutAllowed(EInputQueueOutputState newState) { return true; } // TODO: Change to player state enum
	virtual bool IsStateTransitionInAllowed(EInputQueueOutputState previousState) { return true; }

	virtual void OnStateBeginPlay() {}
	virtual void OnStateEndPlay() {}
	virtual void OnStateInterrupted() {}

	void StartState_Internal();
	void EndState_Internal();

	EInputQueueOutputState GetPlayerStateType() const;

protected:

	EInputQueueOutputState playerStateType;
	TWeakObjectPtr<UMainCharacterData> characterData;
	TWeakObjectPtr<UMainCharacterComponentGroup> characterComponentGroup;

	void EndState();

private:

	bool isStatePlaying;
};
