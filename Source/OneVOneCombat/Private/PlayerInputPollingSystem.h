// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "UserInput.h"
#include "PlayerMainController.h"

#include "PlayerInputPollingSystem.generated.h"



UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPlayerInputPollingSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UPlayerInputPollingSystem();

	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = inputEvent))
	void AddActionToUserInputPollingQueue(UserInputType inputType, TEnumAsByte<EInputEvent> inputEvent);

	const TArray<FUserInput>& GetInputPoll() const;

	void RemoveFromPolling(int32 inputSequenceCount);

	DECLARE_DELEGATE_OneParam(FOnAnInputTriggered, UPlayerInputPollingSystem*);
	FOnAnInputTriggered onAnInputTriggered;

private:

	TArray<FUserInput> inputPoll;

	UPROPERTY(EditAnywhere)
	int8 maxPollSize = 8;
};
