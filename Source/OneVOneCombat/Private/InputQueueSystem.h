// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PlayerInputPollingSystem.h"
#include "InputQueueDataAsset.h"
#include "UserInputType.h"
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

DECLARE_EVENT(UInputQueueSystem, FCommandExecuteEvent);

USTRUCT()
struct FCommandData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	uint8 blockerCount;
	FCommandExecuteEvent commandEvent;
};

UCLASS(Blueprintable)
class UInputQueueSystem : public UActorComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	UInputQueueSystem();

	void BeginPlay() override;
	bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor) override;

	void ConsumeInputs(UPlayerInputPollingSystem* inputPollingSystem);

	template<typename UserClass, typename ...VarTypes>
	FDelegateHandle BindCommand(const FString& command, UserClass* object, typename FCommandExecuteEvent::FDelegate::TMethodPtr<UserClass, VarTypes...> function, VarTypes... vars)
	{
		FCommandExecuteEvent& executeEvent = commandDataMap.FindOrAdd(command).commandEvent;
		return executeEvent.AddUObject(object, function, vars...);
	}

	void RemoveCommand(const FString& command, const FDelegateHandle& delegateHandle);

	UFUNCTION(BlueprintCallable)
	void BlockCommand(const FString& cmd);

	UFUNCTION(BlueprintCallable)
	void UnblockCommand(const FString& cmd);

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FInputQueueData> inputQueueDataArray;

	struct DiscardInputPair
	{
		EUserInputType inputType;
		EInputEvent inputEvent;
	};

	TArray<DiscardInputPair> discardInputPairs;
	TMap<FString, FCommandData> commandDataMap;

	void InvokeCommand(const FString& command);
	void SortInputQueueDataArray();
	void UpdateDiscardInputPair(const UInputQueueDataAsset* const inputQueueDataAsset);
	bool WillCurrentInputBeDiscarded(const FUserInput& userInput);
	bool IsUserInputExpiredForInputQueueAction(const FUserInput& userInput, const FInputQueueAction& inputQueueAction);
};
