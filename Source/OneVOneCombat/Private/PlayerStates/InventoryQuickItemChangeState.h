// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerStateBase.h"

#include "DataSubOwner.h"
#include "DataReadOwner.h"

#include "InventoryQuickItemChangeState.generated.h"

/**
 *
 */

struct FInventoryData;
struct FCharacterStateData;

UCLASS()
class UInventoryQuickItemChangeState : public UPlayerStateBase
{
public:

	GENERATED_BODY()

	UInventoryQuickItemChangeState();

	void OnStateInitialized() override;
	void OnStateBeginPlay() override;
	void OnStateUpdate(float deltaTime) override;
	void OnStateEndPlay(bool isInterrupted, uint32 nextState) override;

private:
	void OnQuickItemChanged();
	void OnQuickItemChangedAnimFinished(const FName& /*Machine Name*/, const FName& /*State Name*/);
	void OnQuickItemChangedEndAnimFinished(const FName& /*Machine Name*/, const FName& /*State Name*/);

	TWeakObjectPtr<UPlayerStateBase> basicMovement;

	DataReadOwner<FInventoryData> inventoryData;
	DataSubOwner<FCharacterStateData> characterStateData;
};
