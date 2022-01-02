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
	const bool isHit = world->ComponentSweepMulti(NO_CONST_REF hitResults, moveableComponent, desiredPosition, sweepEndPosition, rotation, FComponentQueryParams::DefaultComponentQueryParams);

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

FVector UMainCharacterMovementComponent::IterateMovement(FVector inPos, FVector endDelta)
{
	TArray<FHitResult> groundHitResults;
	UWorld* const world = GetWorld();
	const bool isHitGround = world->SweepMultiByChannel(NO_CONST_REF groundHitResults, inPos, endDelta, moveableComponent->GetComponentQuat(), walkableGroundProperties.collisionChannel, moveableComponent->GetCollisionShape(), groundHitSweepQueryParams);

	for (int32 i = 0; i < groundHitResults.Num(); ++i)
	{
		inPos += groundHitResults[i].ImpactNormal * groundHitResults[i].PenetrationDepth;

		if (groundHitResults[i].PenetrationDepth > 0.f)
		{
			velocity = FVector::ZeroVector;
		}
	}

	return inPos;
}

void UMainCharacterMovementComponent::UpdateMoveableComponent(const float deltaTime)
{
	checkf(moveableComponent, TEXT("Moveable component can not be null when UpdateMoveableComponent is invoked"));

	UWorld* const world = GetWorld();

	FVector currentPosition = moveableComponent->GetComponentLocation();
	const FVector gravityAppliedPosition = currentPosition + velocity.ProjectOnTo(gravity.GetNormalizedVector()) * deltaTime + gravity.GetVector() * deltaTime * deltaTime;

	const FHitResult* groundHitResult = nullptr;
	TArray<FHitResult> groundHitResults;

	const bool isHitGround = world->SweepMultiByChannel(NO_CONST_REF groundHitResults, currentPosition + deltaVelocity * deltaTime, gravityAppliedPosition + deltaVelocity * deltaTime, moveableComponent->GetComponentQuat(), walkableGroundProperties.collisionChannel, moveableComponent->GetCollisionShape(), groundHitSweepQueryParams);

	float highest = -99999.f;

	for (int32 i = 0; i < groundHitResults.Num(); ++i)
	{
		if (highest < CalculateDistanceByDirection(gravity.GetNormalizedVector(), groundHitResults[i].ImpactPoint))
		{
			highest = CalculateDistanceByDirection(gravity.GetNormalizedVector(), groundHitResults[i].ImpactPoint);
			groundHitResult = &groundHitResults[i];
		}
	}

	if (isHitGround)
	{
		velocity += gravity.GetVector() * deltaTime * groundHitResult->Time;

		for (int32 i = 0; i < 8; ++i)
		{
			currentPosition = IterateMovement(currentPosition + deltaVelocity * deltaTime, gravityAppliedPosition + deltaVelocity * deltaTime) - deltaVelocity * deltaTime;
		}

		deltaVelocity = FVector::VectorPlaneProject(deltaVelocity.GetSafeNormal(), groundHitResult->ImpactNormal) * deltaVelocity.Size();
	}
	else
	{
		velocity += gravity.GetVector() * deltaTime;
	}

	FVector posChange = (deltaVelocity + velocity) * deltaTime;

	deltaVelocity = FVector::ZeroVector; // TODO: Just testing for a while

	currentPosition += posChange;

	moveableComponent->SetWorldLocationAndRotation(currentPosition, Rotation);
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
