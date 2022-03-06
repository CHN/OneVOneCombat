// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerStateGroupBase.h"
#include "MainCharacter.h"
#include "PlayerStateManager.h"

UPlayerStateGroupBase::UPlayerStateGroupBase()
{
	PrimaryComponentTick.bCanEverTick = false;
	stateGroupType = EPlayerStateGroup::END_OF_ENUM;
}

void UPlayerStateGroupBase::Init(TWeakObjectPtr<AMainCharacter> NewMainCharacter)
{
	mainCharacter = NewMainCharacter;

	CreatePlayerStatesInitially();
	OnInitialized();
}

void UPlayerStateGroupBase::CreatePlayerStatesInitially()
{
	playerStates.Reserve(playerStateTypes.Num());

	for (TSubclassOf<UPlayerStateBase> stateType : playerStateTypes)
	{
		UPlayerStateBase* state = NewObject<UPlayerStateBase>(this, stateType);
		state->Init(mainCharacter);
		playerStates.Push(state);
	}

	playerStateTypes.Empty();
}
