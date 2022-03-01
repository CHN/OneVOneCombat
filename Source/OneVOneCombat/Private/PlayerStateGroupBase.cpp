// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "PlayerStateGroupBase.h"
#include "PlayerStateManager.h"

UPlayerStateGroupBase::UPlayerStateGroupBase()
{
	PrimaryComponentTick.bCanEverTick = false;
	stateGroupType = EPlayerStateGroup::END_OF_ENUM;
}

void UPlayerStateGroupBase::Init(TWeakObjectPtr<AMainCharacter> NewMainCharacter)
{
	mainCharacter = NewMainCharacter;

	OnInitialized();

	for (auto playerState : playerStates)
	{
		playerState->OnStateInitialized();
	}
}
