// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UserInputType.h"

#include "UserInput.generated.h"

// FIXME: Can be renamed for command
USTRUCT(BlueprintType)
struct FUserInput
{
	GENERATED_BODY();

	FUserInput() = default;
	FUserInput(const FString& command);

	UPROPERTY(VisibleAnywhere)
	FString command;

	UPROPERTY(VisibleAnywhere)
	FDateTime timeStamp;
};