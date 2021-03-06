// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryItemInterface.generated.h"

class UInventoryItem;
class UCharacterAnimInstance;

UINTERFACE(MinimalAPI)
class UInventoryItemInterface : public UInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintNativeEvent)
	UInventoryItem* GetItem();

	UFUNCTION(BlueprintNativeEvent)
	void OnItemPickedUp();

	UFUNCTION(BlueprintNativeEvent)
	void OnItemSelected(AMainCharacter* mainCharacter, UCharacterAnimInstance* characterAnimInstance);

	UFUNCTION(BlueprintNativeEvent)
	void OnItemActivated();

	UFUNCTION(BlueprintNativeEvent)
	void OnItemUsed(USkeletalMeshComponent* characterSkeletalMesh);

	UFUNCTION(BlueprintNativeEvent)
	void OnItemDeactivated();

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

	virtual UInventoryItem* GetItem() = 0;
	virtual void OnItemPickedUp();
	virtual void OnItemSelected(AMainCharacter* mainCharacter, UCharacterAnimInstance* characterAnimInstance);
	virtual void OnItemActivated();
	virtual void OnItemUsed(USkeletalMeshComponent* characterSkeletalMesh);
	virtual void OnItemDeactivated();
	virtual void OnItemDropped();
};
