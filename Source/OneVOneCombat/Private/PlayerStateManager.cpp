// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateManager.h"

#include "MainCharacter.h"
#include "CharacterAnimInstance.h"
#include "InputQueueOutputState.h"
#include "MainCharacter/MainCharacterDataAsset.h"
#include "PlayerStateBase.h"
#include "PlayerStateGroupBase.h"
#include "PlayerStateGroupEnum.h"
#include "PlayerStateFlowManager.h"

#include "EditorUtilities.h"

UPlayerStateManager::UPlayerStateManager()
{
	PrimaryComponentTick.bCanEverTick = true;
	stateFlowManager = CreateDefaultSubobject<UPlayerStateFlowManager>("StateFlowManager");
}

void UPlayerStateManager::Init(AMainCharacter* NewMainCharacter)
{
	mainCharacter = NewMainCharacter;
	stateFlowManager->Init(EPlayerState::END_OF_ENUM, EPlayerState::BASIC_MOVEMENT);

	CreateStateGroups();
	InitPlayerStates();

	PushStateGroup(EPlayerStateGroup::DEFAULT_GROUP);
	//PushStateGroup(EPlayerStateGroup::MELEE_ATTACK);

	stateFlowManager->TryToChangeCurrentState(EPlayerState::BASIC_MOVEMENT, "");
}

void UPlayerStateManager::CreateStateGroups()
{
	for (TSubclassOf<UPlayerStateGroupBase> stateGroupType : stateGroupTypes)
	{	
		UPlayerStateGroupBase* stateGroup = NewObject<UPlayerStateGroupBase>(this, stateGroupType);
		stateGroup->Init(mainCharacter);
		stateGroups[stateGroup->GetPlayerStateGroupType()] = stateGroup;
	}

	stateGroupTypes.Empty();
}

void UPlayerStateManager::InitPlayerStates()
{
	auto& stateGroupArray = stateGroups.GetUnderlyingArray();
	for (UPlayerStateGroupBase* stateGroup : stateGroupArray)
	{
		stateGroup->InitPlayerStates();
	}
}

void UPlayerStateManager::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	stateFlowManager->UpdateCurrentState(DeltaTime);
}

void UPlayerStateManager::PushStateGroup(EPlayerStateGroup playerStateGroup)
{
	// Load
	auto loadedStateGroup = stateGroups[playerStateGroup];
	auto& loadedStates = loadedStateGroup->GetPlayerStates();

	TArray<UPlayerStateBase*> previousStatesSnapshot;

	for (UPlayerStateBase* state : loadedStates)
	{
		uint32 stateType = state->GetPlayerState();
		TWeakObjectPtr<UPlayerStateBase> activeState = stateFlowManager->GetState(stateType);

		if (activeState.IsValid())
		{
			activeState->OnStateDeactive();
			previousStatesSnapshot.Push(activeState.Get()); // FIXME: Weak ptr to ptr 
		}

		stateFlowManager->ReplaceStateWith(state);
		state->OnStateActive();
	}

	loadedStateGroups.Emplace(playerStateGroup, std::move(previousStatesSnapshot));
	loadedStateGroup->OnLoaded();
}

void UPlayerStateManager::PopStateGroup()
{
	check(loadedStateGroups.Num() >= 2);

	LoadedStateGroupData& poppedStateGroupData = loadedStateGroups.Top();
	LoadedStateGroupData& loadedStateGroupData = loadedStateGroups[loadedStateGroups.Num() - 2];

	auto unloadedStateGroup = stateGroups[poppedStateGroupData.stateGroup];
	auto& unloadedStates = unloadedStateGroup->GetPlayerStates();

	auto loadedStateGroup = stateGroups[loadedStateGroupData.stateGroup];
	auto& loadedStates = loadedStateGroup->GetPlayerStates();

	// Mark as unloaded
	for (UPlayerStateBase* state : unloadedStates)
	{
		state->OnStateDeactive();
		stateFlowManager->ClearState(state->GetPlayerState());
	}

	for (UPlayerStateBase* state : loadedStates)
	{
		stateFlowManager->ReplaceStateWith(state);
		state->OnStateActive();
	}

	for (UPlayerStateBase* state : poppedStateGroupData.previousStateGroupSnapshot)
	{
		uint32 stateType = state->GetPlayerState();
		TWeakObjectPtr<UPlayerStateBase> activeState = stateFlowManager->GetState(stateType);

		if (!activeState.IsValid())
		{
			stateFlowManager->ReplaceStateWith(state);
			state->OnStateActive();
		}
	}
	
	stateGroups[poppedStateGroupData.stateGroup]->OnUnloaded();
	stateGroups[loadedStateGroupData.stateGroup]->OnLoaded();

	loadedStateGroups.RemoveAt(loadedStateGroups.Num() - 1);
}

UPlayerStateManager::LoadedStateGroupData::LoadedStateGroupData(EPlayerStateGroup NewStateGroup, TArray<UPlayerStateBase*>&& NewPreviousStateGroupSnapshot)
{
	stateGroup = NewStateGroup;
	std::swap(previousStateGroupSnapshot, NewPreviousStateGroupSnapshot);
}
