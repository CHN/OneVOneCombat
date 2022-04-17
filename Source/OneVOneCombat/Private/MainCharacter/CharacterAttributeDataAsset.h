// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterAttributeDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UCharacterAttributeDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	const FVector& GetWalkSpeed() const;
	const FVector& GetSprintSpeed() const;

private:

	UPROPERTY(EditAnywhere)
	FVector walkSpeed;

	UPROPERTY(EditAnywhere)
	FVector sprintSpeed;
};
