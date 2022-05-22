// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerStateBase.h"

#include "DataSubOwner.h"

#include "AttackWeaponPlayerState.generated.h"
/**
 * 
 */

class UPlayerStateFlowManager;
class UWeaponInventoryItemData;
class UWeaponInventoryGlobalItemData;

struct FInventoryData;
struct FCharacterStateData;
struct FInventoryItemInfo;

UCLASS()
class UAttackWeaponPlayerState : public UPlayerStateBase
{
public:
	GENERATED_BODY()

	UAttackWeaponPlayerState();


	bool IsStateInterruptibleByInputStateOutput(EInputQueueOutputState inputOutputState, uint32 newState) override;

private:
	void OnStateInitialized() override;
	void OnStateBeginPlay() override;
	void OnStateUpdate(float deltaTime) override;
	void OnStateEndPlay(bool isInterrupted, uint32 nextState) override;


	void OnAttackInputTriggered(EInputEvent inputEvent);

	FInventoryItemInfo& GetCurrentItemInfo() const;

	DataSubOwner<FInventoryData> inventoryData;
	DataSubOwner<FCharacterStateData> characterStateData;
	FDelegateHandle attackInputDelegateHandle;

	TWeakObjectPtr<UPlayerStateBase> basicMovementState;

	UPROPERTY()
	UWeaponInventoryItemData* weaponItemData;
	UPROPERTY()
	const UWeaponInventoryGlobalItemData* weaponGlobalItemData;
};