// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CachedVector.generated.h"

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