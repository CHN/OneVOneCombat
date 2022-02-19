// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MovementComponentData.h"
#include "WalkableGroundPropertiesData.h"
#include "CharacterInputData.h"
#include "CharacterState.h"
#include "AnimationRelatedData.h"
#include "DataOwner.h"

#include "MainCharacterData.generated.h"

UCLASS(BlueprintType)
class UMainCharacterData : public UObject
{
public:

	GENERATED_BODY()

	UMainCharacterData();

	DataOwner<FCharacterInputData> characterInputDataOwner;
	DataOwner<FMovementComponentData> movementComponentDataOwner;
	DataOwner<FWalkableGroundPropertiesData> walkableGroundPropertiesDataOwner;
	DataOwner<FAnimationRelatedData> animationRelatedDataOwner;

	inline bool IsGrounded() const
	{
		return movementComponentData.isGrounding;
	}

	inline const FVector& GetMovementDelta() const
	{
		return movementComponentData.movementDelta;
	}

	inline const FQuat& GetCurrentRotation() const
	{
		return movementComponentData.currentRotation;
	}

	inline const FVector& GetRawMoveInput() const
	{
		return characterInputData.rawMoveInput;
	}
	
	inline const FVector& GetRawRotateInput() const
	{
		return characterInputData.rawRotateInput;
	}

private:

	UPROPERTY(EditAnywhere, Category = "Input", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCharacterInputData characterInputData;

	UPROPERTY(EditAnywhere, Category = "Movement", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FMovementComponentData movementComponentData;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	FWalkableGroundPropertiesData walkableGroundPropertiesData;
		
	UPROPERTY(EditAnywhere, Category = "Animation", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FAnimationRelatedData animationRelatedData;
};