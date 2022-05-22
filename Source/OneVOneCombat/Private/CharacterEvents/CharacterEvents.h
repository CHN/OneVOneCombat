// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterEvents.generated.h"

/**
 * 
 */

DECLARE_EVENT(UCharacterEvents, FOnInventoryQuickItemChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnimChangedMulticaster, UAnimationAsset*, animationAsset);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateChanged, bool);

UCLASS(BlueprintType)
class UCharacterEvents : public UObject
{
	GENERATED_BODY()

public:

	FOnInventoryQuickItemChanged onInventoryQuickItemChanged;
	FOnStateChanged onSprintDisableStateChanged;
};
