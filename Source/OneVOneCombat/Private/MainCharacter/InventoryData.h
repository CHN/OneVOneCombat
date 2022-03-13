// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InventoryData.generated.h"

/**
 *
 */

USTRUCT()
struct FInventoryItemInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	uint32 count;
};

USTRUCT(BlueprintType)
struct FInventoryData
{
	GENERATED_BODY()


};