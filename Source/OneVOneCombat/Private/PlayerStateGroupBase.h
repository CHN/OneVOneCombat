// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStateGroupEnum.h"
#include "PlayerStateGroupBase.generated.h"

class AMainCharacter;
class UPlayerStateBase;

UCLASS(Abstract, Blueprintable)
class UPlayerStateGroupBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerStateGroupBase();

	void Init(AMainCharacter* NewMainCharacter);
	void InitPlayerStates();

	virtual void OnInitialized() {}
	virtual void OnLoaded() {}
	virtual void OnUnloaded() {}

	inline EPlayerStateGroup GetPlayerStateGroupType() const
	{
		return stateGroupType;
	}

	inline const TArray<UPlayerStateBase*>& GetPlayerStates() const
	{
		return playerStates;
	}

protected:

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UPlayerStateBase>> playerStateTypes;

	UPROPERTY(EditDefaultsOnly)
	EPlayerStateGroup stateGroupType;

	UPROPERTY()
	TArray<UPlayerStateBase*> playerStates;

	UPROPERTY()
	AMainCharacter* mainCharacter;

private:

	void CreatePlayerStatesInitially();
};
