// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InventoryData.generated.h"

/**
 *
 */

class UInventoryItem;

USTRUCT()
struct FInventoryItemInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	uint32 count;

	UPROPERTY(VisibleAnywhere)
	uint32 maxStackCount;

	UPROPERTY(VisibleAnywhere)
	UInventoryItem* item;
};

USTRUCT(BlueprintType)
struct FInventoryData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	uint32 maxItemCount;

	// items.Num() + quickItems.Num() < maxItemCount
	UPROPERTY(VisibleAnywhere)
	TArray<FInventoryItemInfo> items;

	UPROPERTY(VisibleAnywhere)
	TArray<FInventoryItemInfo> quickItems;

	UPROPERTY(EditDefaultsOnly)
	int32 selectedQuickItem = -1;
};