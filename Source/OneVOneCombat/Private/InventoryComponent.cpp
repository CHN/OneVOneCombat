// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "MainCharacter.h"
#include "PlayerInputPollingSystem.h"
#include "UserInputType.h"
#include "MainCharacter/MainCharacterDataAsset.h"
#include "InventoryItem.h"
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

	mainCharacter->GetCharacterData()->inventoryDataOwner.BecomeSubOwner(&inventoryData);
	mainCharacter->GetCharacterData()->characterStateDataOwner.BecomeSubOwner(&characterStateData);

	CreateStartingInventoryItems();
}

void UInventoryComponent::CreateStartingInventoryItems()
{
	for (TSubclassOf<AActor> actorClass : startingItems)
	{
		AActor* actor = GetWorld()->SpawnActor<AActor>(actorClass);
		// FIXME: Need to use a proper way
		UInventoryItem* item = Cast<UInventoryItem>(actor->GetComponentByClass(UInventoryItem::StaticClass())); 
		
		FInventoryItemInfo itemInfo;
		itemInfo.count = 1;
		itemInfo.maxStackCount = 1;
		itemInfo.item = item;

		inventoryData.data->quickItems.Add(itemInfo);
	}
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::OnNextItemSelectInputTriggered(EInputEvent inputEvent)
{
	auto* data = inventoryData.data;

	if (inputEvent != IE_Pressed || 
		data->quickItems.Num() == 0 ||
		characterStateData.data->isQuickItemChanging)
	{
		return;
	}

	++data->selectedQuickItem;

	if (data->selectedQuickItem == data->quickItems.Num())
	{
		data->selectedQuickItem = 0;
	}

	mainCharacter->GetCharacterEvents()->onInventoryQuickItemChanged.Broadcast();
}

void UInventoryComponent::OnPreviousItemSelectInputTriggered(EInputEvent inputEvent)
{
	auto* data = inventoryData.data;

	if (inputEvent != IE_Pressed ||
		data->quickItems.Num() == 0 ||
		characterStateData.data->isQuickItemChanging)
	{
		return;
	}

	--data->selectedQuickItem;

	if (data->selectedQuickItem < 0)
	{
		data->selectedQuickItem = data->quickItems.Num() - 1;
	}

	mainCharacter->GetCharacterEvents()->onInventoryQuickItemChanged.Broadcast();
}
