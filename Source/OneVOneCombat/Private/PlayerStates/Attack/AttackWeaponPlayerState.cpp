// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/Attack/AttackWeaponPlayerState.h"

#include "PlayerStateManager.h"
#include "PlayerStateFlowManager.h"
#include "MainCharacter.h"
#include "MainCharacter/MainCharacterDataAsset.h"
#include "CharacterEvents/CharacterEvents.h"
#include "InventoryItemInterface.h"
#include "InventoryItem.h"
#include "InputQueueSystem.h"
#include "WeaponInventoryItemData.h"
#include "InputQueueOutputState.h"
#include "CommandMap.h"

#include "MainCharacter/InventoryData.h"
#include "MainCharacter/CharacterStateData.h"

UAttackWeaponPlayerState::UAttackWeaponPlayerState()
{
	playerState = EPlayerState::ATTACK;
}

bool UAttackWeaponPlayerState::IsStateInterruptibleByCommand(const FString& command, uint32 newState)
{
	return true;
}

void UAttackWeaponPlayerState::OnStateInitialized()
{
	mainCharacter->GetCharacterData()->inventoryDataOwner.BeSubOwner(&inventoryData);
	mainCharacter->GetCharacterData()->characterStateDataOwner.BeSubOwner(&characterStateData);

	mainCharacter->commandMap->BindCommand("+usePrimaryWeapon", this, &UAttackWeaponPlayerState::OnAttackInputPressed);
	mainCharacter->commandMap->BindCommand("-usePrimaryWeapon", this, &UAttackWeaponPlayerState::OnAttackInputReleased);
}

void UAttackWeaponPlayerState::OnStateBeginPlay()
{
	IInventoryItemInterface::Execute_OnItemActivated(GetCurrentItemInfo().item->GetOwner());

	basicMovementState = playerStateFlowManager->ReuseState(this, EPlayerState::BASIC_MOVEMENT);
	inventoryData->currentFireCounter = 9999;

	weaponItemData = Cast<UWeaponInventoryItemData>(GetCurrentItemInfo().item->GetExtraData());
	check(weaponItemData);

	weaponGlobalItemData = Cast<const UWeaponInventoryGlobalItemData>(GetCurrentItemInfo().item->GetExtraGlobalData());
	check(weaponItemData);

	mainCharacter->GetCharacterEvents()->onSprintDisableStateChanged.Broadcast(true);
}

void UAttackWeaponPlayerState::OnStateUpdate(float deltaTime)
{
	inventoryData->currentFireCounter += deltaTime;

	if (inventoryData->currentFireCounter * weaponGlobalItemData->fireRate >= 1)
	{
		inventoryData->currentFireCounter = 0;
		IInventoryItemInterface::Execute_OnItemUsed(GetCurrentItemInfo().item->GetOwner(), mainCharacter->GetCharacterSkeletalMesh());
	}

	basicMovementState->OnStateUpdate(deltaTime);
}

void UAttackWeaponPlayerState::OnStateEndPlay(bool isInterrupted, uint32 nextState)
{
	characterStateData->isAttacking = false;
	mainCharacter->GetCharacterEvents()->onSprintDisableStateChanged.Broadcast(false);

	if (nextState == EPlayerState::JUMP)
	{
		playerStateFlowManager->SetReturnState(EPlayerState::ATTACK);
	}
}

void UAttackWeaponPlayerState::OnAttackInputPressed()
{
	if (!IsStatePlaying())
	{
		if (playerStateFlowManager->TryToChangeCurrentState(EPlayerState::ATTACK, ""))
		{
			characterStateData->isAttacking = true;
		}
	}
}

void UAttackWeaponPlayerState::OnAttackInputReleased()
{
	if (IsStatePlaying())
	{
		characterStateData->isAttacking = false;
		EndState(EPlayerState::BASIC_MOVEMENT);
	}
}

FInventoryItemInfo& UAttackWeaponPlayerState::GetCurrentItemInfo() const
{
	return inventoryData->quickItems[inventoryData->selectedQuickItem];
}
