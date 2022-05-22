// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/InventoryQuickItemChangeState.h"

#include "MainCharacter.h"
#include "MainCharacter/MainCharacterDataAsset.h"
#include "PlayerStateManager.h"
#include "PlayerStateFlowManager.h"
#include "CharacterEvents/CharacterEvents.h"
#include "InputQueueOutputState.h"
#include "InventoryItem.h"
#include "InventoryItemInterface.h"

UInventoryQuickItemChangeState::UInventoryQuickItemChangeState()
{
	playerState = EPlayerState::QUICK_ITEM_CHANGED;
}

void UInventoryQuickItemChangeState::OnStateInitialized()
{
	mainCharacter->GetCharacterData()->inventoryDataOwner.BeReadOwner(&inventoryData);
	mainCharacter->GetCharacterData()->characterStateDataOwner.BeSubOwner(&characterStateData);
	mainCharacter->GetCharacterEvents()->onInventoryQuickItemChanged.AddUObject(this, &UInventoryQuickItemChangeState::OnQuickItemChanged);

	//mainCharacter->GetCharacterEvents()->animationStateExitEvents["DefaultMachine"]["QuickItemChange"].AddUObject(this, &UInventoryQuickItemChangeState::OnQuickItemChangedAnimFinished);
	//mainCharacter->GetCharacterEvents()->animationStateExitEvents["DefaultMachine"]["QuickItemEnd"].AddUObject(this, &UInventoryQuickItemChangeState::OnQuickItemChangedEndAnimFinished);
}

void UInventoryQuickItemChangeState::OnQuickItemChanged()
{
	playerStateFlowManager->TryToChangeCurrentState(EPlayerState::QUICK_ITEM_CHANGED, EInputQueueOutputState::NONE);
}

void UInventoryQuickItemChangeState::OnStateBeginPlay()
{
	checkf(inventoryData->selectedQuickItem >= 0, TEXT("SelectedQuickItem can not be less than 0"));

	characterStateData->isQuickItemChanging = true;
	basicMovement = playerStateFlowManager->ReuseState(this, EPlayerState::BASIC_MOVEMENT);

	OnQuickItemChangedAnimFinished("", "");
	OnQuickItemChangedEndAnimFinished("", "");
}

void UInventoryQuickItemChangeState::OnStateUpdate(float deltaTime)
{
	basicMovement->OnStateUpdate(deltaTime);
}

void UInventoryQuickItemChangeState::OnQuickItemChangedAnimFinished(const FName& /*Machine Name*/, const FName& /*State Name*/)
{
	const FInventoryItemInfo& selectedItem = inventoryData->quickItems[inventoryData->selectedQuickItem];

	mainCharacter->SpawnItemOnSocket(selectedItem.item->GetMeshSocketName(), selectedItem.item->GetOwner()); // FIXME: Need to use a proper way

	IInventoryItemInterface::Execute_OnItemSelected(selectedItem.item->GetOwner(), mainCharacter, mainCharacter->GetAnimInstance());
}

void UInventoryQuickItemChangeState::OnQuickItemChangedEndAnimFinished(const FName& /*Machine Name*/, const FName& /*State Name*/)
{
	EndState(EPlayerState::BASIC_MOVEMENT);
}

void UInventoryQuickItemChangeState::OnStateEndPlay(bool isInterrupted, uint32 nextState)
{
	characterStateData->isQuickItemChanging = false;
}
