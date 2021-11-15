// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "UserInput.h"

#include "ComboDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UComboDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UComboDataAsset(const FObjectInitializer& ObjectInitializer);

private:

	UPROPERTY(EditAnywhere)
	TArray<UserInputType> inputSequence;

	UPROPERTY(EditAnywhere)
	float minComboTime;

	UPROPERTY(EditAnywhere)
	float maxComboTime;
};
