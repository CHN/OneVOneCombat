// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UserActionAndAxisInputHandler.generated.h"

UCLASS()
class UUserActionAndAxisInputHandler : public UObject
{

	GENERATED_BODY()

public:

	void HandleXAxisNegativeInputPressed();
	void HandleXAxisNegativeInputReleased();
	void HandleXAxisPositiveInputPressed();
	void HandleXAxisPositiveInputReleased();

	void HandleYAxisNegativeInputPressed();
	void HandleYAxisNegativeInputReleased();
	void HandleYAxisPositiveInputPressed();
	void HandleYAxisPositiveInputReleased();

	void HandleXAxisInput(float value);
	void HandleYAxisInput(float value);
	void HandleXAxisInputWithDeltaTime(float value);
	void HandleYAxisInputWithDeltaTime(float value);
	void ResetAxisAccumulation();

	template<typename UserClass>
	void BindAxisFunction(UserClass* object, typename FInputVectorAxisHandlerSignature::template TUObjectMethodDelegate<UserClass>::FMethodPtr method)
	{
		axisHandler.BindUObject(object, method);
	}

private:

	FVector axisActionValue = FVector::ZeroVector;
	FVector axisValue = FVector::ZeroVector;

	FInputVectorAxisHandlerSignature axisHandler;
};