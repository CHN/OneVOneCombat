// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PlayerStateManager.generated.h"

class UPlayerStateBase;
class UPlayerStateGroup;

UENUM()
enum class EPlayerState : uint8
{
	MOVE,
	JUMP,
	ATTACK,

	END_OF_ENUM,
	NONE
};

UENUM()
enum class EPlayerStateGroup : uint8
{
	MELEE_ATTACK,
	WEAPON_ATTACK,

	DEFAULT_GROUP,
	END_OF_ENUM
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPlayerStateManager : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPlayerStateManager();

	void Init(TWeakObjectPtr<AMainCharacter> NewMainCharacter);

	bool TryToChangeCurrentState(EPlayerState nextState, EInputQueueOutputState inputReason);
	TWeakObjectPtr<UPlayerStateBase> ReusePlayerState(const UPlayerStateBase* ownerState, EPlayerState state) const;

	void PushStateGroup(EPlayerStateGroup playerStateGroup);
	void PopStateGroup();

private:

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	template<typename T>
	void CreatePlayerStateGroup(EPlayerStateGroup playerStateGroup);

	template<typename T>
	void CreateBasicPlayerStateGroup(EPlayerStateGroup stateGroupType);

	void OnCurrentStateEndCallback(EPlayerState nextState);

	TWeakObjectPtr<AMainCharacter> mainCharacter;

	TArray<TWeakObjectPtr<UPlayerStateBase>> activeStates;
	TArray<TWeakObjectPtr<UPlayerStateGroup>> stateGroups;
	TWeakObjectPtr<UPlayerStateBase> currentState;

	struct LoadedStateGroupData
	{
		EPlayerStateGroup stateGroup;
		TArray<UPlayerStateBase*> previousStateGroupSnapshot;

		LoadedStateGroupData(EPlayerStateGroup NewStateGroup, TArray<UPlayerStateBase*>&& NewPreviousStateGroupSnapshot);
	};

	TArray<LoadedStateGroupData> loadedStateGroups;
};