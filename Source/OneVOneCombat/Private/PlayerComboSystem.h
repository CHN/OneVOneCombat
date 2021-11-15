// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PlayerInputPollingSystem.h"
#include "ComboDataAsset.h"

#include "PlayerComboSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPlayerComboSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerComboSystem();

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerInputPollingSystem> inputPollingSystem;

	UPROPERTY(EditAnywhere)
	TArray<UComboDataAsset*> activeComboList;
};
