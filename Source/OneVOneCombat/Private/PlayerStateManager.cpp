// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateManager.h"

UPlayerStateManager::UPlayerStateManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerStateManager::BeginPlay()
{
	Super::BeginPlay();
}

bool UPlayerStateManager::TryToChangeState(EInputQueueOutputState targetState)
{
	return false;
}