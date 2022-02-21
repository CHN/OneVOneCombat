// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PlayerStateBase.h"

#include "PlayerStateManager.generated.h"


enum class EInputQueueOutputState : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPlayerStateManager : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPlayerStateManager();

	void Init(TWeakObjectPtr<AMainCharacter> NewMainCharacter);
	void OnInputQueueOutputStateTriggered(EInputQueueOutputState inputOutputState);

	void TryToChangeNextState(EPlayerState nextState);
	const TArray<TWeakObjectPtr<UPlayerStateBase>>& GetPlayerStates() const;

private:

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	template<typename T>
	void CreatePlayerState(EPlayerState playerState);

	template<typename T>
	void CreatePlayerStateWithInput(EPlayerState playerState, EInputQueueOutputState inputQueueOutputState);

	TWeakObjectPtr<AMainCharacter> mainCharacter;

	TArray<TWeakObjectPtr<UPlayerStateBase>> playerStates;
	TArray<TWeakObjectPtr<UPlayerStateBase>> inputOutputPlayerStates;
	TWeakObjectPtr<UPlayerStateBase> currentState;
};