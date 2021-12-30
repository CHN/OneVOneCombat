// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacterMovementComponent.h"

#include "HelperMacros.h"
#include "EditorUtilities.h"	

UMainCharacterMovementComponent::UMainCharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMainCharacterMovementComponent::SetMoveableComponent(UPrimitiveComponent* NewMoveableComponent)
{
	moveableComponent = NewMoveableComponent;
}

static float CalculateDistanceByDirection(const FVector& dir, const FVector& pos)
{
	return FVector::DotProduct(pos, -dir.GetUnsafeNormal());
}

bool UMainCharacterMovementComponent::TryMoveByDelta(const float deltaTime, const FVector& delta, const FQuat& worldRotation)
{
	if (moveableComponent.IsNull())
	{
		return false;
	}
	
	UWorld* const world = GetWorld();

	movementProperties.currentVelocity += movementProperties.gravityConstant * deltaTime;

	FVector currentPosition = moveableComponent->GetComponentLocation();
	const FVector gravitySweepEnd = currentPosition + movementProperties.currentVelocity * deltaTime;

	TArray<FHitResult> gravityHitResults;

	world->SweepMultiByChannel(NO_CONST_REF gravityHitResults, currentPosition, gravitySweepEnd, worldRotation, walkableGroundProperties.collisionChannel, moveableComponent->GetCollisionShape());

	float maxDistance = -1.f;
	FHitResult* topmostHitResult = nullptr;
	bool nonTouchingGround = true;

	for (FHitResult& hitResult : gravityHitResults)
	{
		float distance = CalculateDistanceByDirection(movementProperties.gravityConstant, hitResult.ImpactPoint);

		if (hitResult.Time == 0.f)
		{
			if (distance > maxDistance)
			{
				maxDistance = distance;
				topmostHitResult = &hitResult;
				nonTouchingGround = false;
			}
		}
		else
		{
			topmostHitResult = &hitResult;
			maxDistance = distance;
			break;
		}
	}

	if (nonTouchingGround)
	{
		currentPosition += movementProperties.currentVelocity * deltaTime * (topmostHitResult ? topmostHitResult->Time : 1.f);
	}
	else
	{
		const FVector capsuleBottom = currentPosition + FVector::DownVector * moveableComponent->GetCollisionShape().GetCapsuleHalfHeight();
		float distance = CalculateDistanceByDirection(movementProperties.gravityConstant, capsuleBottom);
		const FVector sweepStart = currentPosition + (distance - maxDistance) * movementProperties.gravityConstant.GetUnsafeNormal();

		FHitResult hitResult;
		const bool isTouching = world->SweepSingleByChannel(NO_CONST_REF hitResult, sweepStart, currentPosition, worldRotation, walkableGroundProperties.collisionChannel, moveableComponent->GetCollisionShape(0.1f));
		
		currentPosition = isTouching ? hitResult.Location : sweepStart;

		movementProperties.currentVelocity = FVector::ZeroVector;
	}

	moveableComponent->SetWorldLocation(currentPosition);

	return true;
}

void UMainCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMainCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}