// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "MainCharacter.h"
#include "PlayerInputPollingSystem.h"
#include "UserInputType.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UInventoryComponent::Init(AMainCharacter* NewMainCharacter)
{
	mainCharacter = NewMainCharacter;

	UPlayerInputPollingSystem* inputPollingSystem = mainCharacter->GetPlayerInputPollingSystem();

	inputPollingSystem->BindInputEvent(EUserInputType::CHANGE_WEAPON_NEXT, this, &UInventoryComponent::OnSelectedItemSwitchedToNext);

	inputPollingSystem->BindInputEvent(EUserInputType::CHANGE_WEAPON_PREVIOUS, this, &UInventoryComponent::OnSelectedItemSwitchedToPrevious);
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UInventoryComponent::OnSelectedItemSwitchedToNext(EInputEvent inputEvent)
{
	
}

void UInventoryComponent::OnSelectedItemSwitchedToPrevious(EInputEvent inputEvent)
{
	
}
