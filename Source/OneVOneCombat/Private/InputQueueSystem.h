// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PlayerInputPollingSystem.h"
#include "InputQueueDataAsset.h"
#include "TEnumArray.h"

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

	DECLARE_EVENT(UInputQueueSystem, FInputQueueSystemEvent)

	// Sets default values for this component's properties
	UInputQueueSystem();

	void BeginPlay() override;

	void ConsumeInputs(UPlayerInputPollingSystem* inputPollingSystem);

	template<typename UserClass>
	FDelegateHandle BindEvent(EInputQueueOutputState state, UserClass* object, typename TMemFunPtrType<false, UserClass, void()>::Type function)
	{
		return events[state].AddUObject(object, function);
	}

	void UnbindEvent(EInputQueueOutputState state, FDelegateHandle handle);

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FInputQueueData> inputQueueDataArray;

	struct DiscardInputPair
	{
		EUserInputType inputType;
		EInputEvent inputEvent;
	};

	TArray<DiscardInputPair> discardInputPairs;
	TEnumArray<FInputQueueSystemEvent, EInputQueueOutputState> events;

	void SortInputQueueDataArray();
	void UpdateDiscardInputPair(const UInputQueueDataAsset* const inputQueueDataAsset);
	bool WillCurrentInputBeDiscarded(const FUserInput& userInput);
	bool IsUserInputExpiredForInputQueueAction(const FUserInput& userInput, const FInputQueueAction& inputQueueAction);
};
