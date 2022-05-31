// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "UserInputType.h"
#include "TEnumArray.h"

#include "InputQueueSystem.generated.h"

class UCommandMap;
class UInputQueueDataAsset;

struct FUserInput;
struct FInputQueueAction;

UCLASS(Blueprintable)
class UInputQueueSystem : public UActorComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	UInputQueueSystem();

	bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor) override;

	UPROPERTY()
	TObjectPtr<UCommandMap> commandMap;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UInputQueueDataAsset>> inputQueueDataArray;

	TArray<FString> discardInputPairs;

	UPROPERTY(EditAnywhere)
	TArray<FUserInput> commandPoll;

	UPROPERTY(EditAnywhere)
	int8 maxPollSize = 16;

	bool ConsumeInputs();
	void UpdateDiscardInputPair(const UInputQueueDataAsset* const inputQueueDataAsset);
	bool WillCurrentInputBeDiscarded(const FUserInput& userInput);
	bool IsUserInputExpiredForInputQueueAction(const FUserInput& userInput, const FInputQueueAction& inputQueueAction);
};
