// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "TEnumArray.h"

#include "PlayerStateManager.generated.h"

enum class EPlayerStateGroup : uint8;
class UPlayerStateBase;
class UPlayerStateGroupBase;
class UPlayerStateFlowManager;

UENUM()
enum class EPlayerState : uint8
{
	BASIC_MOVEMENT,
	MOVE,
	LOOK,
	JUMP,
	ATTACK,

	END_OF_ENUM,
	NONE
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPlayerStateManager : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPlayerStateManager();

	void Init(TWeakObjectPtr<AMainCharacter> NewMainCharacter);

	void PushStateGroup(EPlayerStateGroup playerStateGroup);
	void PopStateGroup();
	
	inline UPlayerStateFlowManager* GetStateFlowManager() const
	{
		return stateFlowManager;
	}

private:

	void CreateStateGroups();
	void InitPlayerStates();

	TWeakObjectPtr<AMainCharacter> mainCharacter;
	TEnumArray<UPlayerStateGroupBase*, EPlayerStateGroup> stateGroups;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UPlayerStateGroupBase>> stateGroupTypes;

	UPROPERTY()
	UPlayerStateFlowManager* stateFlowManager;

	struct LoadedStateGroupData
	{
		EPlayerStateGroup stateGroup;
		TArray<UPlayerStateBase*> previousStateGroupSnapshot;

		LoadedStateGroupData(EPlayerStateGroup NewStateGroup, TArray<UPlayerStateBase*>&& NewPreviousStateGroupSnapshot);
	};

	TArray<LoadedStateGroupData> loadedStateGroups;
};