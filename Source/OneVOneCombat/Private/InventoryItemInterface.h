// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryItemInterface : public UInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintNativeEvent)
	AActor* GetOwnerActor();

	UFUNCTION(BlueprintNativeEvent)
	void OnItemPickedUp();

	UFUNCTION(BlueprintNativeEvent)
	void OnItemStartedToUse();

	UFUNCTION(BlueprintNativeEvent)
	void OnItemActivated();

	UFUNCTION(BlueprintNativeEvent)
	void OnItemDeactivated();

	UFUNCTION(BlueprintNativeEvent)
	void OnItemUseFinished();

	UFUNCTION(BlueprintNativeEvent)
	void OnItemDropped();
};

/**
 * 
 */
class IInventoryItemInterface
{
	GENERATED_BODY()
public:

	virtual AActor* GetOwnerActor() = 0;
	virtual void OnItemPickedUp();
	virtual void OnItemStartedToUse();
	virtual void OnItemActivated();
	virtual void OnItemDeactivated();
	virtual void OnItemUseFinished();
	virtual void OnItemDropped();
};
