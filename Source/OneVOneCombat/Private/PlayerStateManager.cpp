// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateManager.h"

#include "InputQueueOutputState.h"
#include "MainCharacter/MainCharacterData.h"
#include "MainCharacter/CharacterState.h"

#include "PlayerStateBase.h"
#include "PlayerStates/SwordAttackPlayerState.h"

#include "PlayerStateGroup.h"
#include "PlayerStateGroups/DefaultStateGroup.h"

#include "EditorUtilities.h"

UPlayerStateManager::UPlayerStateManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

template<typename T>
void UPlayerStateManager::CreatePlayerStateGroup(EPlayerStateGroup stateGroupType)
{
	TObjectPtr<T> stateGroup = NewObject<T>(this);
	stateGroup->Init(mainCharacter);
	stateGroups[static_cast<uint8>(stateGroupType)] = stateGroup;
}

template<typename T>
void UPlayerStateManager::CreateBasicPlayerStateGroup(EPlayerStateGroup stateGroupType)
{
	TWeakObjectPtr<UPlayerStateGroup> playerStateGroup = UPlayerStateGroup::CreateBasicPlayerStateGroup<T>(stateGroupType, this, mainCharacter);
	stateGroups[static_cast<uint8>(stateGroupType)] = playerStateGroup;
}

void UPlayerStateManager::Init(TWeakObjectPtr<AMainCharacter> NewMainCharacter)
{
	mainCharacter = NewMainCharacter;

	activeStates.SetNum(static_cast<uint8>(EPlayerState::END_OF_ENUM));
	stateGroups.SetNum(static_cast<uint8>(EPlayerStateGroup::END_OF_ENUM));

	CreatePlayerStateGroup<UDefaultStateGroup>(EPlayerStateGroup::DEFAULT_GROUP);
	CreateBasicPlayerStateGroup<USwordAttackPlayerState>(EPlayerStateGroup::MELEE_ATTACK);

	PushStateGroup(EPlayerStateGroup::DEFAULT_GROUP);
	PushStateGroup(EPlayerStateGroup::MELEE_ATTACK);
	TryToChangeCurrentState(EPlayerState::MOVE, EInputQueueOutputState::NONE);
}

bool UPlayerStateManager::TryToChangeCurrentState(EPlayerState nextState, EInputQueueOutputState inputReason)
{
	const bool isCurrentStateValid = currentState.IsValid();
	const bool isCurrentStatePlaying = isCurrentStateValid && currentState->IsStatePlaying();
	
	if (isCurrentStatePlaying)
	{
		const bool isInterruptible = currentState->IsStateInterruptible(nextState);
		const bool isInputInterruptible = currentState->IsStateInterruptibleByInputStateOutput(inputReason, nextState);

		if (!isInterruptible && !isInputInterruptible)
		{
			return false;
		}
	}

	auto newState = activeStates[static_cast<uint8>(nextState)];

	const EPlayerState currentPlayerState = isCurrentStateValid ? currentState->GetPlayerState() : EPlayerState::NONE;

	const bool isTransitionAllowed = newState->IsStateTransitionInAllowed(currentPlayerState);
	const bool isInputTransitionAllowed = newState->IsStateTransitionInAllowedByInputStateOutput(inputReason, currentPlayerState);

	if (!isTransitionAllowed || !isInputTransitionAllowed)
	{
		return false;
	}

	if (isCurrentStateValid)
	{
		currentState->EndState_Internal();
	}

	newState->oneTimeStateEndCallback.BindUObject(this, &UPlayerStateManager::OnCurrentStateEndCallback);
	newState->StartState_Internal();
	currentState = newState;

	return true;
}

TWeakObjectPtr<UPlayerStateBase> UPlayerStateManager::ReusePlayerState(const UPlayerStateBase* ownerState, EPlayerState state) const
{
	auto reusedPlayerState = activeStates[static_cast<uint8>(state)];
	reusedPlayerState->OnStateReused(ownerState->GetPlayerState());

	return reusedPlayerState;
}

void UPlayerStateManager::PushStateGroup(EPlayerStateGroup playerStateGroup)
{
	// Load
	auto loadedStateGroup = stateGroups[static_cast<uint8>(playerStateGroup)];
	auto& loadedStates = loadedStateGroup->GetPlayerStates();

	TArray<UPlayerStateBase*> previousStatesSnapshot;

	for (UPlayerStateBase* state : loadedStates)
	{
		int8 stateTypeInt = static_cast<uint8>(state->GetPlayerState());
		
		if (activeStates[stateTypeInt].IsValid())
		{
			activeStates[stateTypeInt]->OnStateDeactive();
			previousStatesSnapshot.Push(activeStates[stateTypeInt].Get()); // FIXME: Weak ptr to ptr 
		}

		activeStates[stateTypeInt] = state;
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

	auto unloadedStateGroup = stateGroups[static_cast<uint8>(poppedStateGroupData.stateGroup)];
	auto& unloadedStates = unloadedStateGroup->GetPlayerStates();

	auto loadedStateGroup = stateGroups[static_cast<uint8>(loadedStateGroupData.stateGroup)];
	auto& loadedStates = loadedStateGroup->GetPlayerStates();

	// Mark as unloaded
	for (UPlayerStateBase* state : unloadedStates)
	{
		int8 stateTypeInt = static_cast<uint8>(state->GetPlayerState());
		state->OnStateDeactive();
		activeStates[stateTypeInt] = nullptr;
	}

	for (UPlayerStateBase* state : loadedStates)
	{
		int8 stateTypeInt = static_cast<uint8>(state->GetPlayerState());
		activeStates[stateTypeInt] = state;
		state->OnStateActive();
	}

	for (UPlayerStateBase* state : poppedStateGroupData.previousStateGroupSnapshot)
	{
		int8 stateTypeInt = static_cast<uint8>(state->GetPlayerState());

		if (!activeStates[stateTypeInt].IsValid())
		{
			activeStates[stateTypeInt] = state;
			state->OnStateActive();
		}
	}
	
	stateGroups[static_cast<uint8>(poppedStateGroupData.stateGroup)]->OnUnloaded();
	stateGroups[static_cast<uint8>(loadedStateGroupData.stateGroup)]->OnLoaded();

	loadedStateGroups.RemoveAt(loadedStateGroups.Num() - 1);
}

void UPlayerStateManager::OnCurrentStateEndCallback(EPlayerState nextState)
{
	TryToChangeCurrentState(nextState, EInputQueueOutputState::NONE);
}

void UPlayerStateManager::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	currentState->OnStateUpdate(DeltaTime);
}

UPlayerStateManager::LoadedStateGroupData::LoadedStateGroupData(EPlayerStateGroup NewStateGroup, TArray<UPlayerStateBase*>&& NewPreviousStateGroupSnapshot)
{
	stateGroup = NewStateGroup;
	std::swap(previousStateGroupSnapshot, NewPreviousStateGroupSnapshot);
}
