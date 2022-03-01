// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PlayerStateBase.h"

#include "PlayerStateManager.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPlayerStateManager : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPlayerStateManager();

	void Init(TWeakObjectPtr<AMainCharacter> NewMainCharacter);

	bool TryToChangeCurrentState(EPlayerState nextState, EInputQueueOutputState inputReason);
	TWeakObjectPtr<UPlayerStateBase> ReusePlayerState(const UPlayerStateBase* ownerState, EPlayerState state) const;

private:

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	template<typename T>
	void CreatePlayerState(EPlayerState playerState);

	void OnCurrentStateEndCallback(EPlayerState nextState);

	TWeakObjectPtr<AMainCharacter> mainCharacter;

	TArray<TWeakObjectPtr<UPlayerStateBase>> playerStates;
	TWeakObjectPtr<UPlayerStateBase> currentState;
};