// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "MainCharacter.h"
#include "PlayerInputPollingSystem.h"
#include "UserInputType.h"
#include "MainCharacter/MainCharacterDataAsset.h"
#include "InventoryItem.h"
#include "InventoryItemInterface.h"
#include "CharacterEvents/CharacterEvents.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UInventoryComponent::Init(AMainCharacter* NewMainCharacter)
{
	mainCharacter = NewMainCharacter;

	UPlayerInputPollingSystem* inputPollingSystem = mainCharacter->GetPlayerInputPollingSystem();

	inputPollingSystem->BindInputEvent(EUserInputType::CHANGE_WEAPON_NEXT, this, &UInventoryComponent::OnNextItemSelectInputTriggered);

	inputPollingSystem->BindInputEvent(EUserInputType::CHANGE_WEAPON_PREVIOUS, this, &UInventoryComponent::OnPreviousItemSelectInputTriggered);

	mainCharacter->GetCharacterData()->inventoryDataOwner.BeSubOwner(&inventoryData);
	mainCharacter->GetCharacterData()->characterStateDataOwner.BeReadOwner(&characterStateData);

	CreateStartingInventoryItems();
}

void UInventoryComponent::CreateStartingInventoryItems()
{
	for (TSubclassOf<AActor> actorClass : startingItems)
	{
		AActor* actor = GetWorld()->SpawnActor<AActor>(actorClass);
		
		UInventoryItem* item = IInventoryItemInterface::Execute_GetItem(actor);
		
		FInventoryItemInfo itemInfo;
		itemInfo.count = 1;
		itemInfo.maxStackCount = 1;
		itemInfo.item = item;

		inventoryData->quickItems.Add(itemInfo);
	}
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::OnNextItemSelectInputTriggered(EInputEvent inputEvent)
{
	if (inputEvent != IE_Pressed || 
		inventoryData->quickItems.Num() == 0 ||
		characterStateData->isQuickItemChanging)
	{
		return;
	}

	++inventoryData->selectedQuickItem;

	if (inventoryData->selectedQuickItem == inventoryData->quickItems.Num())
	{
		inventoryData->selectedQuickItem = 0;
	}

	mainCharacter->GetCharacterEvents()->onInventoryQuickItemChanged.Broadcast();
}

void UInventoryComponent::OnPreviousItemSelectInputTriggered(EInputEvent inputEvent)
{
	if (inputEvent != IE_Pressed ||
		inventoryData->quickItems.Num() == 0 ||
		characterStateData->isQuickItemChanging)
	{
		return;
	}

	--inventoryData->selectedQuickItem;

	if (inventoryData->selectedQuickItem < 0)
	{
		inventoryData->selectedQuickItem = inventoryData->quickItems.Num() - 1;
	}

	mainCharacter->GetCharacterEvents()->onInventoryQuickItemChanged.Broadcast();
}
