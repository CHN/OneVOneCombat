// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PlayerInputPollingSystem.h"
#include "InputQueueDataAsset.h"

#include "InputQueueSystem.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UInputQueueSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInputQueueSystem();

	void ConsumeInputs(UPlayerInputPollingSystem* inputPollingSystem);

private:

	UPROPERTY(EditAnywhere)
	TArray<UInputQueueDataAsset*> activeComboList;
};
