// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacterMovementComponent.h"

#include "HelperMacros.h"
#include "EditorUtilities.h"	
#include "DrawDebugHelpers.h"

UMainCharacterMovementComponent::UMainCharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	gravity = FCachedVector(0, 0, -980.f);
	groundHitSweepQueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(groundHitSweepQueryParams), false);
}

void UMainCharacterMovementComponent::SetMoveableComponent(UPrimitiveComponent* NewMoveableComponent)
{
	moveableComponent = NewMoveableComponent;
}

static float CalculateDistanceByDirection(const FVector& dir, const FVector& pos)
{
	return FVector::DotProduct(pos, dir.GetUnsafeNormal()) * -1.f;
}

void UMainCharacterMovementComponent::MoveByDelta(const float duration, const FVector& delta, const FQuat& rotation)
{
	checkf(moveableComponent, TEXT("Moveable component can not be null when MoveByDelta is invoked"));

	const FVector desiredPosition = moveableComponent->GetComponentLocation() + accumulatedDelta;
	const FVector sweepEndPosition = desiredPosition + delta;

	UWorld* const world = GetWorld();

	TArray<FHitResult> hitResults;
	const bool isHit = world->ComponentSweepMulti(NO_CONST_REF hitResults, moveableComponent, desiredPosition, sweepEndPosition, rotation, FComponentQueryParams::DefaultComponentQueryParams); // TODO: Consider using single sweep

	if (isHit)
	{
		const FHitResult& firstHitResult = hitResults[0];
		FVector adjustedDelta = firstHitResult.Location - desiredPosition;

		accumulatedDelta += adjustedDelta;
	}
	else
	{
		accumulatedDelta += delta;
	}
	
	deltaVelocity = accumulatedDelta / duration;

	accumulatedDelta = FVector::ZeroVector; // TODO: Just testing for a while
	Rotation = rotation;
}

FVector UMainCharacterMovementComponent::FindNonCollidingClosestPosition(const FVector& initialPosition, const FVector& sweepEndPosition) /*const*/ // TODO: Update logic and enable const again
{
	FVector currentPosition = initialPosition;

	TArray<FHitResult> hitResults;
	const bool isHitting = GetWorld()->SweepMultiByChannel(NO_CONST_REF hitResults, currentPosition, sweepEndPosition, moveableComponent->GetComponentQuat(), moveableComponent->GetCollisionObjectType(), moveableComponent->GetCollisionShape(), groundHitSweepQueryParams);

	if (!isHitting)
	{
		return sweepEndPosition;
	}

	for (const FHitResult& hitResult : hitResults)
	{
		currentPosition += hitResult.ImpactNormal * hitResult.PenetrationDepth;

		if (hitResult.PenetrationDepth > 0.f)
		{
			velocity = FVector::ZeroVector; // TODO: A workaround for testing
		}
	}

	return currentPosition;
}

void UMainCharacterMovementComponent::UpdateMoveableComponent(const float deltaTime)
{
	checkf(moveableComponent, TEXT("Moveable component can not be null when UpdateMoveableComponent is invoked"));

	velocity += gravity.GetVector() * deltaTime;

	FVector currentPosition = moveableComponent->GetComponentLocation();
	FVector sweepEndPosition = currentPosition + deltaVelocity * deltaTime;

	for (uint8 i = 0; i < movementIterationCount; ++i)
	{
		currentPosition = FindNonCollidingClosestPosition(currentPosition, sweepEndPosition);
	}

	FHitResult groundHitResult;
	const bool isHitGround = GetWorld()->SweepSingleByChannel(NO_CONST_REF groundHitResult, currentPosition, currentPosition + velocity * deltaTime, moveableComponent->GetComponentQuat(), walkableGroundProperties.collisionChannel, moveableComponent->GetCollisionShape(), groundHitSweepQueryParams);

	if (isHitGround)
	{
		deltaVelocity = FVector::VectorPlaneProject(deltaVelocity.GetSafeNormal(), groundHitResult.ImpactNormal) * deltaVelocity.Size();
	}

	currentPosition = moveableComponent->GetComponentLocation();
	FVector movementEndPosition = currentPosition + velocity * deltaTime;

	for (uint8 i = 0; i < movementIterationCount; ++i)
	{
		currentPosition = FindNonCollidingClosestPosition(currentPosition, movementEndPosition);
	}

	movementEndPosition = currentPosition + deltaVelocity * deltaTime;

	for (uint8 i = 0; i < movementIterationCount; ++i)
	{
		currentPosition = FindNonCollidingClosestPosition(currentPosition, movementEndPosition);
	}

	moveableComponent->SetWorldLocationAndRotation(currentPosition, Rotation);

	deltaVelocity = FVector::ZeroVector; // TODO: Workaround for testing
}

void UMainCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateMoveableComponent(DeltaTime);
}

void UMainCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void FCachedVector::SetVector(float X, float Y, float Z)
{
	originalVector.X = X;
	originalVector.Y = Y;
	originalVector.Z = Z;

	UpdateCache();
}

void FCachedVector::SetVector(const FVector& val)
{
	originalVector = val;
	UpdateCache();
}

void FCachedVector::UpdateCache()
{
	normalizedVector = originalVector.GetSafeNormal();
	sqrMagnitude = originalVector.SizeSquared();
	magnitude = FMath::Sqrt(sqrMagnitude);
}
