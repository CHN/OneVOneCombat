// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MainCharacterComponentGroup.generated.h"

/**
 * 
 */

class UMainCharacterMovementComponent;

UCLASS()
class UMainCharacterComponentGroup : public UObject
{
public:
	GENERATED_BODY()

	UMainCharacterComponentGroup();

	TWeakObjectPtr<UMainCharacterMovementComponent> GetMovementComponent() const;

private:

	UPROPERTY(EditAnywhere)
	UMainCharacterMovementComponent* movementComponent;
};
