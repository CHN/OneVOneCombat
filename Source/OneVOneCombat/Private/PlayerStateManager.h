// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PlayerStateBase.h"

#include "PlayerStateManager.generated.h"


enum class EInputQueueOutputState : uint8;
class UMainCharacterData;
class UMainCharacterMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPlayerStateManager : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPlayerStateManager();

	void Init(TObjectPtr<UMainCharacterData> characterData, UMainCharacterMovementComponent* tempMovementComp);
	void OnInputQueueOutputStateTriggered(EInputQueueOutputState state);

private:

	TArray<TObjectPtr<UPlayerStateBase>> playerStates;
};
