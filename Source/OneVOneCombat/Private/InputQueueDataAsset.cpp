// Fill out your copyright notice in the Description page of Project Settings.


#include "InputQueueDataAsset.h"

UInputQueueDataAsset::UInputQueueDataAsset(const FObjectInitializer& ObjectInitializer)
	: UDataAsset(ObjectInitializer)
{

}

const TArray<FInputQueueAction>& UInputQueueDataAsset::GetInputActions() const
{
	return InputActions;
}

EInputQueueOutputState UInputQueueDataAsset::GetInputQueueOutputState() const
{
	return InputQueueOutputState;
}
