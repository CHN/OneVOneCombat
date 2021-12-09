// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PlayerInputPollingSystem.h"
#include "InputQueueDataAsset.h"

#include "InputQueueSystem.generated.h"

USTRUCT(BlueprintType)
struct FInputQueueData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EUserInputType> inputType = EUserInputType::END_OF_ENUM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UInputQueueDataAsset*> inputQueueDataAssets;
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
	TArray<FInputQueueData> inputQueueDataArray;

	struct DiscardInputPair
	{
		EUserInputType inputType;
		EInputEvent inputEvent;
	};

	TArray<DiscardInputPair> discardInputPairs;

	void SortInputQueueDataArray();
	void UpdateDiscardInputPair(const UInputQueueDataAsset* const inputQueueDataAsset);
	bool WillCurrentInputBeDiscarded(const FUserInput& userInput);
	bool IsUserInputExpiredForInputQueueAction(const FUserInput& userInput, const FInputQueueAction& inputQueueAction);
};
