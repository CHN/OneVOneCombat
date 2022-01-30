// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CachedVector.h"
#include "DataSubOwner.h"
#include "DataInlineSubOwner.h"
#include "MainCharacter/MovementComponentData.h"
#include "MainCharacter/WalkableGroundPropertiesData.h"

#include "MainCharacterMovementComponent.generated.h"

UCLASS(Blueprintable)
class UMainCharacterMovementComponent : public UActorComponent, public DataSubOwner<FMovementComponentData>
{
	GENERATED_BODY()

public:
	
	UMainCharacterMovementComponent();

	UFUNCTION(BlueprintCallable)
	void SetMoveableComponent(UPrimitiveComponent* NewMoveableComponent);

	void MoveByDelta(const float deltaTime, const FVector& delta, const FQuat& rotation, bool constrainInputToGround = true);
	void AddVelocity(const FVector& NewVelocity);

	bool IsGrounding() const { return data->isGrounding; }
	bool IsMovementBeingApplied() const;

private:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateMoveableComponent(const float deltaTime);

	DataInlineSubOwner<FWalkableGroundPropertiesData> walkableGroundPropertiesSubOwner;

	TObjectPtr<UPrimitiveComponent> moveableComponent;

	FCollisionQueryParams groundHitSweepQueryParams;

	FVector FindNonCollidingClosestPosition(const FVector& initialPosition, const FVector& sweepEndPosition);

	struct LedgeReturnData
	{
		bool isDetected;
		float ledgeHeight;
	};

	LedgeReturnData IsLedgeDetected(const FVector& centerPoint, const FVector& impactPoint) const;
};
