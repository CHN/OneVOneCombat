// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateGroups/DefaultStateGroup.h"

#include "PlayerStateManager.h"

#include "PlayerStates/JumpPlayerState.h"
#include "PlayerStates/MovementPlayerState.h"

// Sets default values for this component's properties
UDefaultStateGroup::UDefaultStateGroup()
	: Super()
{
	stateGroupType = EPlayerStateGroup::DEFAULT_GROUP;
}

void UDefaultStateGroup::OnInitialized()
{
	CreatePlayerState<UMovementPlayerState>(EPlayerState::MOVE);
	CreatePlayerState<UJumpPlayerState>(EPlayerState::JUMP);
}
