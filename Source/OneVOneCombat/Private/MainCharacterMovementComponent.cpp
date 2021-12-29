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

bool UMainCharacterMovementComponent::TryMoveByDelta(const float deltaTime, const FVector& delta, const FQuat& worldRotation)
{
	if (moveableComponent.IsNull())
	{
		return false;
	}

	UWorld* const currentWorld = GetWorld();

	FVector sweepStartPosition = moveableComponent->GetComponentLocation();

	FVector groundHitNormal = FVector::UpVector;
	const float hitResultTime = CalculateAppliableGravityDelta(deltaTime, &groundHitNormal);
	
	if (hitResultTime == 0.f)
	{
		movementProperties.currentVelocity.Z = 0;
	}
	else
	{
		movementProperties.currentVelocity += movementProperties.gravityConstant * deltaTime;
	}

	sweepStartPosition += movementProperties.currentVelocity * deltaTime * hitResultTime;

	const FVector projectedDelta = FVector::VectorPlaneProject(delta, groundHitNormal).GetSafeNormal() * delta.Size();

	const FVector sweepEndPosition = sweepStartPosition + projectedDelta;

	FHitResult hitResult;

	if (currentWorld->SweepSingleByChannel(NO_CONST_REF hitResult, sweepStartPosition + groundHitNormal * 0.1f, sweepEndPosition + groundHitNormal * 0.1f, worldRotation, sweepCollisionChannel, moveableComponent->GetCollisionShape()))
	{
		if (FVector::DotProduct(projectedDelta, hitResult.Normal) > 0.f)
		{
			moveableComponent->SetWorldLocationAndRotation(sweepEndPosition, worldRotation, false);
		}
		else
		{
			const FVector projectedDelta2 = FVector::VectorPlaneProject(projectedDelta, hitResult.Normal);

			moveableComponent->SetWorldLocationAndRotation(sweepStartPosition + projectedDelta2, worldRotation, false);
		}
		
		return true;
	}

	moveableComponent->SetWorldLocationAndRotation(sweepEndPosition, worldRotation, false);

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

float UMainCharacterMovementComponent::CalculateAppliableGravityDelta(const float deltaTime, FVector* outGroundHitNormal)
{
	UWorld* const currentWorld = GetWorld();

	FVector sweepEndPosition = moveableComponent->GetComponentLocation();

	sweepEndPosition += movementProperties.gravityConstant * 0.01;

	TArray<FHitResult> hitResults;

	if (currentWorld->SweepMultiByChannel(NO_CONST_REF hitResults, moveableComponent->GetComponentLocation(), sweepEndPosition, FQuat::Identity, walkableGroundProperties.collisionChannel, moveableComponent->GetCollisionShape()))
	{
		float smallestDot = -9999;
		FHitResult* h = nullptr;

		for (FHitResult& hitResult : hitResults)
		{
			const float facingDotProduct = FVector::DotProduct(moveableComponent->GetUpVector(), hitResult.Normal);

			if (hitResult.Location.Z > smallestDot)
			{
				smallestDot = hitResult.Location.Z;
				h = &hitResult;
			}
		}

		if (h == nullptr)
		{
			return 1.f;
		}

		/*if (walkableGroundProperties.minSlopeDotProduct <= facingDotProduct && facingDotProduct <= walkableGroundProperties.maxSlopeDotProduct)
		{

		}*/

		*outGroundHitNormal = h->Normal;

		return h->Time;
	}

	return 1.f;
}