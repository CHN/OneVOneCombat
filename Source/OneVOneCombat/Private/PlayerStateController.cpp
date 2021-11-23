// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateController.h"

UPlayerStateController::UPlayerStateController()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerStateController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerStateController::BeginPlay()
{
	Super::BeginPlay();

	playerStates.SetNumZeroed(static_cast<uint8>(EPlayerState::END_OF_ENUM));

	InitPlayerStates();
}

void UPlayerStateController::AddPlayerState(UPlayerStateObject* newPlayerState)
{
	EPlayerState state = newPlayerState->GetPlayerState();

	check(state != EPlayerState::END_OF_ENUM);
	check(newPlayerState->GetOuter() == this);
	check(playerStates[static_cast<uint8>(state)] == nullptr);

	playerStates[static_cast<uint8>(state)] = newPlayerState;
}

void UPlayerStateController::InitPlayerStates()
{

}
