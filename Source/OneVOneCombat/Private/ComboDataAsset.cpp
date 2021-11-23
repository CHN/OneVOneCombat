// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboDataAsset.h"

UComboDataAsset::UComboDataAsset(const FObjectInitializer& ObjectInitializer)
	: UDataAsset(ObjectInitializer)
{

}

const TArray<UserInputType>& UComboDataAsset::GetInputSequence() const
{
	return inputSequence;
}

EPlayerState UComboDataAsset::GetTargetPlayerState() const
{
	return targetPlayerState;
}
