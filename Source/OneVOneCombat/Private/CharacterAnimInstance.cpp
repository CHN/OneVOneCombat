// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "MainCharacter.h"
#include "CharacterEvents/AnimationStateEventTableRow.h"
#include "CharacterEvents/CharacterEvents.h"
#include "CharacterAnimSetterDataAsset.h"
#include "EditorUtilities.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	AMainCharacter* mainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());

	if (mainCharacter)
	{
		characterData = mainCharacter->GetCharacterData();
		characterEvents = mainCharacter->GetCharacterEvents();
	}

	Super::NativeInitializeAnimation();
}

void UCharacterAnimInstance::SetCharacterAnimations(UCharacterAnimSetterDataAsset* characterAnimDataAsset)
{
	idlePoseAnim = characterAnimDataAsset->idlePoseAnim;
	aimPoseAnim = characterAnimDataAsset->aimPoseAnim;
	runLoopPoseAnim = characterAnimDataAsset->runLoopPoseAnim;
}