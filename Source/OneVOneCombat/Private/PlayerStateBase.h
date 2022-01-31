// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PlayerStateBase.generated.h"

enum class EInputQueueOutputState : uint8;
class UMainCharacterData;

UCLASS(Abstract)
class UPlayerStateBase : public UActorComponent
{
public:
	
	GENERATED_BODY()

	UPlayerStateBase();

	void InitBase(TObjectPtr<UMainCharacterData> NewCharacterData);

	virtual bool IsStateTransitionAllowedToNewState(EInputQueueOutputState outputState) { return true; } // TODO: Change to player state enum
	virtual bool IsStateTransitionAllowedToThisState(EInputQueueOutputState outputState) { return true; }

	virtual void OnStateBeginPlay() {}
	virtual void OnStateEndPlay() {}
	virtual void OnStateInterrupted() {}

	void StartState_Internal();

	EInputQueueOutputState GetPlayerStateType() const;

protected:

	EInputQueueOutputState playerStateType;
	TObjectPtr<UMainCharacterData> characterData;

	void EndState();

private:

	bool isStatePlaying;
};
