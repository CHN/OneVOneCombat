// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MainCharacterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class AMainCharacterPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere)
	FVector rawMoveInput;

	UPROPERTY(VisibleAnywhere)
	FVector rawLookInput;
};
