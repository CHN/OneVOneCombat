// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacterMovementComponent.h"

#include "MainCharacter.h"
#include "MainCharacter/MainCharacterDataAsset.h"

#include "HelperMacros.h"
#include "EditorUtilities.h"	
#include "DrawDebugHelpers.h"

UMainCharacterMovementComponent::UMainCharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	groundHitSweepQueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(groundHitSweepQueryParams), false);
}

void UMainCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	auto* characterData = Cast<AMainCharacter>(GetOwner())->GetCharacterData();

	characterData->movementComponentDataOwner.BecomeSubOwner(this);
	characterData->walkableGroundPropertiesDataOwner.BecomeSubOwner(&walkableGroundPropertiesSubOwner);

	data->gravity = FCachedVector(0, 0, -980.f);
}

void UMainCharacterMovementComponent::SetMoveableComponent(UPrimitiveComponent* NewMoveableComponent)
{
	moveableComponent = NewMoveableComponent;
	groundHitSweepQueryParams.ClearIgnoredActors();
	groundHitSweepQueryParams.AddIgnoredActor(moveableComponent->GetOwner());

	data->movementTargetPosition = FVector::ZeroVector;
	data->movementDelta = FVector::ZeroVector;
	data->movementDuration = 0.f;
	data->currentRotation = moveableComponent->GetComponentRotation().Quaternion();
}

void UMainCharacterMovementComponent::SetVerticalRotationComponent(USceneComponent* NewVerticalRotationComponent)
{
	verticalRotationComponent = NewVerticalRotationComponent;
}

void UMainCharacterMovementComponent::MoveByDelta(const float duration, const FVector& delta, bool constrainInputToGround)
{
	checkf(moveableComponent, TEXT("Moveable component can not be null when MoveByDelta is invoked"));
	
	data->isMovementBeginApplied = true;

	data->movementDelta = delta;
	data->movementTargetPosition = moveableComponent->GetComponentLocation() + data->movementDelta;
	data->movementDuration = duration;
	data->currentDuration = data->movementDuration;
	data->constrainInputToGround = constrainInputToGround;
}

void UMainCharacterMovementComponent::RotateByDelta(const FQuat& deltaRotation) // FIXME: Duration may be needed
{
	data->currentRotation = deltaRotation * data->currentRotation;
}

void UMainCharacterMovementComponent::RotateByDeltaXY(const FQuat& deltaRotationX, const FQuat& deltaRotationY) // FIXME: Duration may be needed
{
	data->currentRotation = deltaRotationX * data->currentRotation * deltaRotationY;
}

void UMainCharacterMovementComponent::RotateVerticalRotationComponent(const FQuat& deltaRotation)
{
	FQuat rotation = verticalRotationComponent->GetComponentRotation().Quaternion() * deltaRotation;

	FQuat swing;
	FQuat twist;

	rotation.ToSwingTwist(FVector::ForwardVector, swing, twist);

	FVector twistEuler = twist.Euler();
	twistEuler.X = FMath::ClampAngle(twistEuler.X, data->verticalRotationMinDegree, data->verticalRotationMaxDegree);
	twist = FQuat::MakeFromEuler(twistEuler);

	verticalRotationComponent->SetWorldRotation(swing * twist);
}

void UMainCharacterMovementComponent::AddVelocity(const FVector& NewVelocity)
{
	data->velocity += NewVelocity;
}

FVector UMainCharacterMovementComponent::FindNonCollidingClosestPosition(const FVector& initialPosition, const FVector& sweepEndPosition) /*const*/ // TODO: Update logic and enable const again
{
	TArray<FHitResult> hitResults;
		
	const bool isHitting = GetWorld()->SweepMultiByChannel(NO_CONST_REF hitResults, initialPosition, sweepEndPosition, moveableComponent->GetComponentQuat(), walkableGroundPropertiesSubOwner.data->collisionChannel, moveableComponent->GetCollisionShape(), groundHitSweepQueryParams);

	if (!isHitting)
	{
		return sweepEndPosition;
	}

	FVector avgPosition = FVector::ZeroVector;

	for (const FHitResult& hitResult : hitResults)
	{
		avgPosition += hitResult.Location + hitResult.ImpactNormal * (hitResult.PenetrationDepth + 0.1f);
	}

	return avgPosition / hitResults.Num();
}

UMainCharacterMovementComponent::LedgeReturnData UMainCharacterMovementComponent::IsLedgeDetected(const FVector& centerPoint, const FVector& impactPoint) const
{
	const auto moveableCompShape = moveableComponent->GetCollisionShape();
	const FVector capsuleBottomSphereCenter = centerPoint + moveableComponent->GetUpVector() * -moveableCompShape.GetCapsuleAxisHalfLength();

	const FVector impactVector = capsuleBottomSphereCenter - impactPoint;

	const float ledgeAmount = FVector::DotProduct(impactVector.GetUnsafeNormal(), moveableComponent->GetUpVector());

	LedgeReturnData ledgeReturnData;
	
	ledgeReturnData.isDetected = ledgeAmount < 1.f && ledgeAmount > 0.3f; // TODO: Should be changed with editor ui
	ledgeReturnData.ledgeHeight = FMath::Max(moveableCompShape.GetCapsuleRadius() - FVector::DotProduct(impactVector, moveableComponent->GetUpVector()), 10.f);

	return ledgeReturnData;
}

bool UMainCharacterMovementComponent::IsMovementBeingApplied() const
{
	return data->isMovementBeginApplied;
}

void UMainCharacterMovementComponent::UpdateMoveableComponent(const float deltaTime)
{
	checkf(moveableComponent, TEXT("Moveable component can not be null when UpdateMoveableComponent is invoked"));

	data->velocity += data->gravity.GetVector() * deltaTime;

	const FVector currentPos = moveableComponent->GetComponentLocation();
	FVector updatedPos = currentPos;

	FVector deltaVelocity;

	data->currentDuration -= deltaTime;

	if (data->currentDuration <= 0.f)
	{
		if (data->isMovementBeginApplied)
		{
			deltaVelocity = (data->movementTargetPosition - currentPos) / deltaTime;
			data->isMovementBeginApplied = false;
		}
		else
		{
			deltaVelocity = FVector::ZeroVector;
		}
	}
	else
	{
		deltaVelocity = data->movementDelta / data->movementDuration; // Handle frame rate changes
	}

	UWorld* const world = GetWorld();

	FHitResult groundHitResult(1.f);

	const FVector gravityAppliedPos = updatedPos + data->velocity * deltaTime;
	const FVector groundInflation = data->gravity.GetNormalizedVector() * -2.f; // 0.1 => inflate for not getting hit by ground

	if (world->SweepSingleByChannel(NO_CONST_REF groundHitResult, updatedPos, gravityAppliedPos - groundInflation * 1.1f /* FIXME: Handle multiplying 1.1 with different way */, data->currentRotation, moveableComponent->GetCollisionObjectType(), moveableComponent->GetCollisionShape(), groundHitSweepQueryParams))
	{
		updatedPos = groundHitResult.Location + groundInflation;

		if (data->constrainInputToGround)
		{
			deltaVelocity = FVector::VectorPlaneProject(deltaVelocity, groundHitResult.ImpactNormal);
		}

		data->velocity = FVector::ZeroVector; // Easy way for now

		data->isGrounding = true;
	}
	else
	{
		updatedPos = gravityAppliedPos;

		data->isGrounding = false;
	}

	const FVector movementAppliedPos = updatedPos + deltaVelocity * deltaTime;

	FHitResult movementHitResult(1.f);

	if (world->SweepSingleByChannel(NO_CONST_REF movementHitResult, updatedPos, movementAppliedPos, data->currentRotation, moveableComponent->GetCollisionObjectType(), moveableComponent->GetCollisionShape(), groundHitSweepQueryParams))
	{
		const float nonAppliedDeltaPosition = deltaVelocity.Size() * deltaTime - (updatedPos - movementHitResult.Location).Size();
		updatedPos = movementHitResult.Location + movementHitResult.ImpactNormal * 0.1f;
		
		if (nonAppliedDeltaPosition > 0)
		{
			LedgeReturnData ledgeReturnData = IsLedgeDetected(updatedPos, movementHitResult.ImpactPoint);

			if (ledgeReturnData.isDetected)
			{
				updatedPos += moveableComponent->GetUpVector() * ledgeReturnData.ledgeHeight * deltaTime * 12.f; // TODO: Fix use of ledge height instead of using climbing velocity
			}
			else
			{
				FVector alignedImpactNormal;
				float verticalImpactAmount = FVector::DotProduct(movementHitResult.ImpactNormal, moveableComponent->GetUpVector());

				alignedImpactNormal = movementHitResult.ImpactNormal - verticalImpactAmount * moveableComponent->GetUpVector();

				alignedImpactNormal.Normalize();

				const FVector adjustedUnitDeltaVelocity = FVector::VectorPlaneProject(deltaVelocity.GetSafeNormal(), alignedImpactNormal);

				updatedPos += adjustedUnitDeltaVelocity * nonAppliedDeltaPosition;
			}
		}
	}
	else
	{
		updatedPos = movementAppliedPos;
	}
		
	updatedPos = FindNonCollidingClosestPosition(currentPos, updatedPos);

	//DEBUG_CAPSULE(moveableComponent);

	moveableComponent->SetWorldLocationAndRotation(updatedPos, data->currentRotation);
}

void UMainCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateMoveableComponent(DeltaTime);
}