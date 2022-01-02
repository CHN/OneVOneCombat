// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MainCharacterMovementComponent.generated.h"

USTRUCT()
struct FWalkableGroundProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float minSlopeDotProduct = 0.95f;

	UPROPERTY(EditAnywhere)
	float maxSlopeDotProduct = 1.f;

	UPROPERTY(VisibleAnywhere)
	float sweepGroundAvoidanceDistance = 1.f;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> collisionChannel;
};

USTRUCT()
struct FCachedVector
{
public:

	GENERATED_BODY()

	FCachedVector() = default;
	FCachedVector(const FVector& vec) { SetVector(vec); }
	FCachedVector(float X, float Y, float Z) { SetVector(X, Y, Z); }

	void SetVector(float X, float Y, float Z);
	void SetVector(const FVector& val);

	inline const FVector& GetVector() const { return originalVector; }
	
	inline const FVector& GetNormalizedVector() const { return normalizedVector; }
	inline float GetMagnitude() const { return magnitude; }
	inline float GetSqrMagnitude() const { return sqrMagnitude; }

	inline float X() { return originalVector.X; }
	inline float Y() { return originalVector.Y; }
	inline float Z() { return originalVector.Z; }

private:

	void UpdateCache();

	UPROPERTY(VisibleAnywhere, Category = "Cached Vector", meta = (AllowPrivateAccess = "true"))
	FVector originalVector;

	UPROPERTY(VisibleAnywhere, Category = "Cached Vector", meta = (AllowPrivateAccess = "true"))
	FVector normalizedVector;

	UPROPERTY(VisibleAnywhere, Category = "Cached Vector", meta = (AllowPrivateAccess = "true"))
	float magnitude;

	UPROPERTY(VisibleAnywhere, Category = "Cached Vector", meta = (AllowPrivateAccess = "true"))
	float sqrMagnitude;
};

UCLASS(Blueprintable)
class UMainCharacterMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UMainCharacterMovementComponent();

	UFUNCTION(BlueprintCallable)
	void SetMoveableComponent(UPrimitiveComponent* NewMoveableComponent);

	void MoveByDelta(const float duration, const FVector& delta, const FQuat& rotation);

private:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateMoveableComponent(const float deltaTime);

	UPROPERTY(EditAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FWalkableGroundProperties walkableGroundProperties;

	UPROPERTY(VisibleAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FCachedVector gravity;

	UPROPERTY(VisibleAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FVector velocity;

	UPROPERTY(VisibleAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FVector deltaVelocity;

	UPROPERTY(VisibleAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FVector accumulatedDelta;
	FQuat Rotation;

	TObjectPtr<UPrimitiveComponent> moveableComponent;

	FCollisionQueryParams groundHitSweepQueryParams;

	FVector IterateMovement(FVector inPos, FVector endDelta);
};
