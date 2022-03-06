// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerStateGroup.h"
#include "MainCharacter.h"
#include "PlayerStateManager.h"

UPlayerStateGroup::UPlayerStateGroup()
{
	PrimaryComponentTick.bCanEverTick = false;
	stateGroupType = EPlayerStateGroup::END_OF_ENUM;
}

void UPlayerStateGroup::Init(TWeakObjectPtr<AMainCharacter> NewMainCharacter)
{
	mainCharacter = NewMainCharacter;

	OnInitialized();

	for (auto playerState : playerStates)
	{
		playerState->OnStateInitialized();
	}
}