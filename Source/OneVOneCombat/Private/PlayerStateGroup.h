// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStateGroup.generated.h"


class AMainCharacter;
class UPlayerStateBase;
enum class EPlayerStateGroup : uint8;
enum class EPlayerState : uint8;

// FIXME: Consider a different way to handle groups

UCLASS(meta=(BlueprintSpawnableComponent))
class UPlayerStateGroup : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerStateGroup();

	void Init(TWeakObjectPtr<AMainCharacter> NewMainCharacter);

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

	template<typename T>
	static TWeakObjectPtr<UPlayerStateGroup> CreateBasicPlayerStateGroup(EPlayerStateGroup stateGroupType, UObject* outer, TWeakObjectPtr<AMainCharacter> NewMainCharacter)
	{
		TWeakObjectPtr<UPlayerStateGroup> playerStateGroup = NewObject<UPlayerStateGroup>(outer, UPlayerStateGroup::StaticClass());
		playerStateGroup->stateGroupType = stateGroupType;
		playerStateGroup->Init(NewMainCharacter);
		playerStateGroup->CreatePlayerState<T>();
		return playerStateGroup;
	}

protected:

	template<typename T>
	void CreatePlayerState()
	{
		TObjectPtr<T> state = NewObject<T>(this);
		state->Init(mainCharacter);
		playerStates.Push(state);
	}

	EPlayerStateGroup stateGroupType;
	TArray<UPlayerStateBase*> playerStates;
	TWeakObjectPtr<AMainCharacter> mainCharacter;
};
