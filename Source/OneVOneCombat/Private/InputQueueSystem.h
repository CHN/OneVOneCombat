// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PlayerInputPollingSystem.h"
#include "InputQueueDataAsset.h"

#include "InputQueueSystem.generated.h"

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

	struct DiscardInputPair
	{
		EUserInputType inputType;
		EInputEvent inputEvent;
	};

	TArray<DiscardInputPair> discardInputPairs;

	void UpdateDiscardInputPair(const UInputQueueDataAsset* const inputQueueDataAsset);
	bool WillCurrentInputBeDiscarded(const FUserInput& userInput);
};
