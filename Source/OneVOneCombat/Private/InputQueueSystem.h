// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PlayerInputPollingSystem.h"
#include "InputQueueDataAsset.h"

#include "InputQueueSystem.generated.h"

USTRUCT()
struct FDiscardInputPair
{
	GENERATED_BODY()

	EUserInputType inputType;
	EInputEvent inputEvent;
};

UCLASS(Blueprintable)
class UInputQueueSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInputQueueSystem();

	void BeginPlay() override;

	void ConsumeInputs(UPlayerInputPollingSystem* inputPollingSystem);

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UInputQueueDataAsset*> inputQueueDataAssets;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FDiscardInputPair> discardInputPairs;

	void UpdateDiscardInputPairForQueueFound(const UInputQueueDataAsset* const inputQueueDataAsset);
	void UpdateDiscardInputPair(const UInputQueueDataAsset* const inputQueueDataAsset);
	bool WillCurrentInputBeDiscarded(const FUserInput& userInput);
};
