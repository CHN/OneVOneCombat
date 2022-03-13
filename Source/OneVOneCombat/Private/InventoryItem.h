// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InventoryItem.generated.h"

class IInventoryItemInterface;

/**
 * 
 */

UENUM()
enum class EItemType : uint8
{
	MeleeWeapon,
	Weapon
};

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class UInventoryItem : public UActorComponent
{
public:

	GENERATED_BODY()

	UFUNCTION(BlueprintPure)
	TScriptInterface<IInventoryItemInterface> GetItemInterface() const { return itemInterface; }

	UFUNCTION(BlueprintCallable)
	void SetItemInterface(TScriptInterface<IInventoryItemInterface> NewItemInterface) { itemInterface = NewItemInterface; }

	UFUNCTION(BlueprintPure)
	EItemType GetItemType() const { return itemType; }

	UFUNCTION(BlueprintCallable)
	void SetItemType(EItemType NewItemType) { itemType = NewItemType; }

	UFUNCTION(BlueprintPure)
	const FName& GetMeshSocketName() const { return meshSocketName; }

	UFUNCTION(BlueprintCallable)
	void SetMeshSocketName(const FName& NewMeshSocketName) { meshSocketName = NewMeshSocketName; }

private:
	
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = "GetItemType", BlueprintSetter = "SetItemType")
	EItemType itemType;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = "GetMeshSocketName", BlueprintSetter = "SetMeshSocketName")
	FName meshSocketName;

	UPROPERTY(VisibleAnywhere, BlueprintGetter = "GetItemInterface", BlueprintSetter = "SetItemInterface")
	TScriptInterface<IInventoryItemInterface> itemInterface;
};
