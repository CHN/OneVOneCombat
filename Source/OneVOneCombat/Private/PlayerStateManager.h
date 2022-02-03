// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PlayerStateBase.h"

#include "PlayerStateManager.generated.h"


enum class EInputQueueOutputState : uint8;
class UMainCharacterData;
class UMainCharacterMovementComponent;
class UMainCharacterComponentGroup;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPlayerStateManager : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPlayerStateManager();

	void Init(TWeakObjectPtr<UMainCharacterData> characterData, TWeakObjectPtr<UMainCharacterComponentGroup> characterComponentGroup);
	void OnInputQueueOutputStateTriggered(EInputQueueOutputState inputOutputState);

private:

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<TWeakObjectPtr<UPlayerStateBase>> playerStates;
	TArray<TWeakObjectPtr<UPlayerStateBase>> inputOutputPlayerStates;
	TWeakObjectPtr<UPlayerStateBase> currentState;
};
