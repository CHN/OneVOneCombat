// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MovementComponentData.h"
#include "WalkableGroundPropertiesData.h"
#include "CharacterInputData.h"
#include "AnimationRelatedData.h"
#include "CharacterStateData.h"
#include "InventoryData.h"
#include "DataOwner.h"

#include "MainCharacterDataAsset.generated.h"

UCLASS(BlueprintType)
class UMainCharacterDataAsset : public UDataAsset
{
public:

	GENERATED_BODY()

	DataOwner<FCharacterInputData> characterInputDataOwner;
	DataOwner<FMovementComponentData> movementComponentDataOwner;
	DataOwner<FWalkableGroundPropertiesData> walkableGroundPropertiesDataOwner;
	DataOwner<FAnimationRelatedData> animationRelatedDataOwner;
	DataOwner<FCharacterStateData> characterStateDataOwner;
	DataOwner<FInventoryData> inventoryDataOwner;

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

	inline const FVector& GetLastVelocity() const
	{
		return movementComponentData.lastVelocity;
	}

	inline const FVector& GetVelocity() const
	{
		return movementComponentData.velocity;
	}

private:

	void PostLoad() override;

	UPROPERTY(EditAnywhere, Category = "Input", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCharacterInputData characterInputData;

	UPROPERTY(EditAnywhere, Category = "Movement", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FMovementComponentData movementComponentData;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	FWalkableGroundPropertiesData walkableGroundPropertiesData;
		
	UPROPERTY(EditAnywhere, Category = "Animation", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FAnimationRelatedData animationRelatedData;
		
	UPROPERTY(EditAnywhere, Category = "State", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCharacterStateData characterStateData;

	UPROPERTY(EditAnywhere, Category = "Inventory", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FInventoryData inventoryData;
};