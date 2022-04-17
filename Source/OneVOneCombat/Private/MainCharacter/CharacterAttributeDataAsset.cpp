// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter/CharacterAttributeDataAsset.h"

const FVector& UCharacterAttributeDataAsset::GetWalkSpeed() const
{
	return walkSpeed;
}

const FVector& UCharacterAttributeDataAsset::GetSprintSpeed() const
{
	return sprintSpeed;
}
