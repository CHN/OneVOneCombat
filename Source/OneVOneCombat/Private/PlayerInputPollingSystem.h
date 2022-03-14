// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "UserInput.h"
#include "PlayerMainController.h"
#include "TEnumArray.h"

#include "PlayerInputPollingSystem.generated.h"


DECLARE_EVENT_OneParam(UPlayerInputPollingSystem, FPlayerInputPollingInputEvent, EInputEvent)

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPlayerInputPollingSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UPlayerInputPollingSystem();

	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = inputEvent))
	void AddActionToUserInputPollingQueue(EUserInputType inputType, TEnumAsByte<EInputEvent> inputEvent);

	const TArray<FUserInput>& GetInputPoll() const;

	void RemoveFromPolling(int32 inputSequenceCount);
	void RemoveFromPollingAt(int32 index);

	DECLARE_DELEGATE_OneParam(FOnAnInputTriggered, UPlayerInputPollingSystem*);
	FOnAnInputTriggered onAnInputTriggered;

	UPROPERTY(EditAnywhere)
	TArray<FUserInput> inputPoll;

	template<typename UserClass>
	FDelegateHandle BindInputEvent(EUserInputType inputType, UserClass* object, typename TMemFunPtrType<false, UserClass, void(EInputEvent)>::Type function)
	{
		return inputEvents[inputType].AddUObject(object, function);
	}

	void UnbindInputEvent(EUserInputType inputType, const FDelegateHandle& handle);

private:

	UPROPERTY(EditAnywhere)
	int8 maxPollSize = 16;

	TEnumArray<FPlayerInputPollingInputEvent, EUserInputType> inputEvents;
};
