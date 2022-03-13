// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "MainCharacter.h"
#include "CharacterEvents/AnimationStateEventTableRow.h"
#include "CharacterEvents/CharacterEvents.h"
#include "EditorUtilities.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	AMainCharacter* mainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	
	if (mainCharacter)
	{
		characterData = mainCharacter->GetCharacterData();
		BindAnimationEvents(mainCharacter->GetAnimationStateEventDataTable(), mainCharacter->GetCharacterEvents());
	}

	Super::NativeInitializeAnimation();
}

void UCharacterAnimInstance::BindAnimationEvents(const UDataTable* eventTable, TWeakObjectPtr<UCharacterEvents> characterEvents)
{
	for (auto eventRowIt : eventTable->GetRowMap())
	{
		FAnimationStateEventTableRow* eventRow = (FAnimationStateEventTableRow*)eventRowIt.Value;
		
		auto& machineName = eventRow->machineName;
		auto& stateName = eventRow->stateName;

		switch (eventRow->animationStateEventType)
		{
		case EAnimationStateEventType::Entry:
			AddNativeStateEntryBinding(eventRow->machineName, eventRow->stateName, FOnGraphStateChanged::CreateLambda([=](const FAnimNode_StateMachine&, int32, int32)
				{
					characterEvents->animationStateEntryEvents[machineName][stateName].Broadcast(machineName, stateName);
				}));
			break;
		case EAnimationStateEventType::Exit:
			AddNativeStateExitBinding(eventRow->machineName, eventRow->stateName, FOnGraphStateChanged::CreateLambda([=](const FAnimNode_StateMachine&, int32, int32)
				{
					characterEvents->animationStateExitEvents[machineName][stateName].Broadcast(machineName, stateName);
				}));
			break;
		default:
			check(false);
		}
	}
}
