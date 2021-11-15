// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "UserInput.h"
#include "PlayerMainController.h"

#include "PlayerInputPollingSystem.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPlayerInputPollingSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UPlayerInputPollingSystem();

	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = actionMapState))
	void AddActionToUserInputPollingQueue(EPlayerState targetState, TEnumAsByte<ActionMappingState> actionMapState);

private:

	TQueue<FUserInput, EQueueMode::Spsc> inputPoll;

	UPROPERTY(EditAnywhere)
	int8 maxPollSize = 8;

	int8 currentPollCount = 0;
};
