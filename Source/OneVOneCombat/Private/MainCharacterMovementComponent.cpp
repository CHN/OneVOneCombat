// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacterMovementComponent.h"

#include "HelperMacros.h"
#include "EditorUtilities.h"	
#include "DrawDebugHelpers.h"

UMainCharacterMovementComponent::UMainCharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	gravity = FCachedVector(0, 800, -980.f);
	groundHitSweepQueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(groundHitSweepQueryParams), false);
}

void UMainCharacterMovementComponent::SetMoveableComponent(UPrimitiveComponent* NewMoveableComponent)
{
	moveableComponent = NewMoveableComponent;
	groundHitSweepQueryParams.ClearIgnoredActors();
	groundHitSweepQueryParams.AddIgnoredActor(moveableComponent->GetOwner());

	gravity = FCachedVector(0, 0, -980.f);
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

	FHitResult hitResult;
	const bool isHit = world->SweepSingleByChannel(NO_CONST_REF hitResult, desiredPosition, sweepEndPosition, rotation, moveableComponent->GetCollisionObjectType(), moveableComponent->GetCollisionShape(), groundHitSweepQueryParams); // TODO: Consider using single sweep

	if (isHit)
	{
		FVector adjustedDelta = FVector::VectorPlaneProject(delta, hitResult.ImpactNormal);;

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

void UMainCharacterMovementComponent::AddVelocity(const FVector& NewVelocity)
{
	velocity += NewVelocity;
}

FVector UMainCharacterMovementComponent::FindNonCollidingClosestPosition(const FVector& initialPosition, const FVector& sweepEndPosition) /*const*/ // TODO: Update logic and enable const again
{
	TArray<FHitResult> hitResults;
		
	const bool isHitting = GetWorld()->SweepMultiByChannel(NO_CONST_REF hitResults, initialPosition, sweepEndPosition, moveableComponent->GetComponentQuat(), walkableGroundProperties.collisionChannel, moveableComponent->GetCollisionShape(), groundHitSweepQueryParams);

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

void UMainCharacterMovementComponent::UpdateMoveableComponent(const float deltaTime)
{
	checkf(moveableComponent, TEXT("Moveable component can not be null when UpdateMoveableComponent is invoked"));

	velocity += gravity.GetVector() * deltaTime;

	FVector updatedPos = moveableComponent->GetComponentLocation();

	UWorld* const world = GetWorld();

	FHitResult groundHitResult(1.f);

	const FVector gravityAppliedPos = updatedPos + velocity * deltaTime;
	const FVector groundInflation = gravity.GetNormalizedVector() * -2.f; // 0.1 => inflate for not getting hit by ground

	if (world->SweepSingleByChannel(NO_CONST_REF groundHitResult, updatedPos, gravityAppliedPos - groundInflation, Rotation, moveableComponent->GetCollisionObjectType(), moveableComponent->GetCollisionShape(), groundHitSweepQueryParams))
	{
		updatedPos = FindNonCollidingClosestPosition(updatedPos, groundHitResult.Location + groundInflation);
		deltaVelocity = FVector::VectorPlaneProject(deltaVelocity, groundHitResult.ImpactNormal);
		velocity = FVector::ZeroVector; // Easy way for now

		isGrounding = true;
	}
	else
	{	
		updatedPos = gravityAppliedPos;

		isGrounding = false;
	}

	const FVector movementAppliedPos = updatedPos + deltaVelocity * deltaTime;

	FHitResult movementHitResult(1.f);

	if (world->SweepSingleByChannel(NO_CONST_REF movementHitResult, updatedPos, movementAppliedPos, Rotation, moveableComponent->GetCollisionObjectType(), moveableComponent->GetCollisionShape(), groundHitSweepQueryParams))
	{
		FVector alignedImpactNormal;
		float verticalImpactAmount = FVector::DotProduct(movementHitResult.ImpactNormal, moveableComponent->GetUpVector());

		if (verticalImpactAmount < 0.f)
		{
			alignedImpactNormal = movementHitResult.ImpactNormal - verticalImpactAmount * moveableComponent->GetUpVector();
			alignedImpactNormal = alignedImpactNormal.GetSafeNormal();
		}
		else
		{
			alignedImpactNormal = movementHitResult.ImpactNormal;
		}

		deltaVelocity = FVector::VectorPlaneProject(deltaVelocity, alignedImpactNormal);

		updatedPos = FindNonCollidingClosestPosition(updatedPos, updatedPos + deltaVelocity * deltaTime);
	}
	else
	{
		updatedPos = movementAppliedPos;
	}

	moveableComponent->SetWorldLocationAndRotation(updatedPos, Rotation);
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
