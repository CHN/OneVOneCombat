// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStateManager.generated.h"


enum class EInputQueueOutputState : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPlayerStateManager : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPlayerStateManager();

	bool TryToChangeState(EInputQueueOutputState targetState);

private:

	virtual void BeginPlay() override;
};
