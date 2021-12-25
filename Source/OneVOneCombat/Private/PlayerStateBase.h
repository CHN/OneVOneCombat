// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PlayerStateBase.generated.h"

enum class EInputQueueOutputState : uint8;

UCLASS(Abstract)
class UPlayerStateBase : public UActorComponent
{
public:
	
	GENERATED_BODY()

	virtual bool TryToSwitchState(EInputQueueOutputState outputState) { check(0 && "Inherit and implement the logic"); return false; }

	EInputQueueOutputState GetPlayerStateType() const;

protected:

	EInputQueueOutputState playerStateType;

private:

};
