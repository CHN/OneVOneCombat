// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InventoryData.generated.h"

/**
 *
 */

class UInventoryItem;

USTRUCT(BlueprintType)
struct FInventoryItemInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 count;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 maxStackCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventoryItem* item;
};

USTRUCT(BlueprintType)
struct FInventoryData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 maxItemCount;

	// items.Num() + quickItems.Num() < maxItemCount
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FInventoryItemInfo> items;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FInventoryItemInfo> quickItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 selectedQuickItem = -1;

	float currentFireCounter;
};