// Fill out your copyright notice in the Description page of Project Settings.


#include "InputQueueDataAsset.h"

UInputQueueDataAsset::UInputQueueDataAsset(const FObjectInitializer& ObjectInitializer)
	: UDataAsset(ObjectInitializer)
{

}

const TArray<UserInputType>& UInputQueueDataAsset::GetInputSequence() const
{
	return inputSequence;
}

EPlayerState UInputQueueDataAsset::GetTargetPlayerState() const
{
	return targetPlayerState;
}
