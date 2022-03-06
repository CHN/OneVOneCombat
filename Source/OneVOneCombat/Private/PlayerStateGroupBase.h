// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStateGroupEnum.h"
#include "PlayerStateGroupBase.generated.h"

class AMainCharacter;
class UPlayerStateBase;
enum class EPlayerState : uint8;

UCLASS(Abstract, Blueprintable)
class UPlayerStateGroupBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerStateGroupBase();

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

protected:

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UPlayerStateBase>> playerStateTypes;

	UPROPERTY(EditDefaultsOnly)
	EPlayerStateGroup stateGroupType;

	TArray<UPlayerStateBase*> playerStates;
	TWeakObjectPtr<AMainCharacter> mainCharacter;

private:

	void CreatePlayerStatesInitially();
};
