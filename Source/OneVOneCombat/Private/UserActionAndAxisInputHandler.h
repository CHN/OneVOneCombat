// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UserInput.h"

#include "UserActionAndAxisInputHandler.generated.h"

DECLARE_DELEGATE_OneParam(FInputAxisHandlerSignature, float);

UCLASS()
class UUserActionAndAxisInputHandler : public UObject
{

	GENERATED_BODY()

public:

	void HandleNegativeInputPressed();
	void HandleNegativeInputReleased();
	void HandlePositiveInputPressed();
	void HandlePositiveInputReleased();
	void HandleAxisInput(float value);
	void ResetAxisAccumulation();

	template<typename UserClass>
	void BindAxisFunction(UserClass* object, typename FInputAxisHandlerSignature::template TUObjectMethodDelegate<UserClass>::FMethodPtr method)
	{
		axisHandler = MakeShared<FInputAxisHandlerSignature>(FInputAxisHandlerSignature::CreateUObject(object, method));
	}

private:

	inline float GetAxisValue()
	{
		return UserInputUtilities::ConvertActionToAxisInputByBools(bIsNegativePressed, bIsPositivePressed);
	}

	bool bIsNegativePressed = false;
	bool bIsPositivePressed = false;

	float axisValue = 0.f;

	TSharedPtr<FInputAxisHandlerSignature> axisHandler;
};