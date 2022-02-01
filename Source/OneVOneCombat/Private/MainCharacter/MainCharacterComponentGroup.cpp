// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter/MainCharacterComponentGroup.h"

#include "MainCharacterMovementComponent.h"

UMainCharacterComponentGroup::UMainCharacterComponentGroup()
	: Super()
{
	movementComponent = CreateDefaultSubobject<UMainCharacterMovementComponent>("MovementComponent");
}

TWeakObjectPtr<UMainCharacterMovementComponent> UMainCharacterComponentGroup::GetMovementComponent() const
{
	return MakeWeakObjectPtr(movementComponent);
}
