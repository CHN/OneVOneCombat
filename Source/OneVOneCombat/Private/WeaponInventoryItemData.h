// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WeaponInventoryItemData.generated.h"

/**
 * 
 */

UENUM()
enum class EFireMode : uint8
{
	SINGLE,
	BURST,
	AUTO
};

UCLASS(BlueprintType)
class UWeaponInventoryItemData : public UDataAsset
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EFireMode fireMode;
	UPROPERTY(EditAnywhere)
	uint32 currentBulletInMag;
	UPROPERTY(EditAnywhere)
	uint32 currentMagCount;
};

UCLASS(BlueprintType)
class UWeaponInventoryGlobalItemData : public UDataAsset
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	uint32 magCount;
	UPROPERTY(EditAnywhere)
	uint32 magSize;
	UPROPERTY(EditAnywhere)
	uint32 burstBulletCount;
	UPROPERTY(EditAnywhere)
	float fireRate;
	UPROPERTY(EditAnywhere)
	float burstFireRate;
};
