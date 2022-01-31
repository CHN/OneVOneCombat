// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MovementComponentData.h"
#include "WalkableGroundPropertiesData.h"
#include "DataOwner.h"

#include "MainCharacterData.generated.h"

UCLASS()
class UMainCharacterData : public UObject
{
public:

	GENERATED_BODY()

	UMainCharacterData();

	DataOwner<FMovementComponentData> movementComponentDataOwner;
	DataOwner<FWalkableGroundPropertiesData> walkableGroundPropertiesDataOwner;

	inline bool IsGrounded() const
	{
		return movementComponentData.isGrounding;
	}

private:

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	FMovementComponentData movementComponentData;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	FWalkableGroundPropertiesData walkableGroundPropertiesData;
};