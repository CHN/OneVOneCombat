// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateController.h"

UPlayerStateController::UPlayerStateController()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerStateController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	testx.Broadcast();
}

void UPlayerStateController::BeginPlay()
{
	Super::BeginPlay();

	playerStates.SetNumZeroed(static_cast<uint8>(EPlayerState::END_OF_ENUM));

	InitPlayerStates();

	// First state should be idle-like thing and character can't be stateless
	currentActiveState = playerStates[0];
}

void UPlayerStateController::AddPlayerState(UPlayerStateObject* newPlayerState)
{
	EPlayerState state = UPlayerStateObject::Execute_GetPlayerState(newPlayerState);

	check(state != EPlayerState::END_OF_ENUM);
	check(newPlayerState->GetOuter() == this);
	check(playerStates[static_cast<uint8>(state)] == nullptr);

	playerStates[static_cast<uint8>(state)] = newPlayerState;
}


void UPlayerStateController::InitPlayerStates()
{
	// TODO: Implement here with special state implementations

	AddPlayerState(NewObject<UPlayerStateObject>(this, TEXT("Idle State")));

	
}

bool UPlayerStateController::TryToSwitchState(EPlayerState targetStateEnum)
{
	check(currentActiveState);

	EPlayerState currentState = UPlayerStateObject::Execute_GetPlayerState(currentActiveState);

	auto* const targetState = playerStates[static_cast<uint8>(targetStateEnum)];

	const bool isSwitched = UPlayerStateObject::Execute_IsStateSwitchable(currentActiveState, currentState) &&
							UPlayerStateObject::Execute_TryToExecuteState(currentActiveState, currentState);
	return isSwitched;
}
