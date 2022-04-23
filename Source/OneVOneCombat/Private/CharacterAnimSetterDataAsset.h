// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CharacterAnimSetterDataAsset.generated.h"

/**
 * 
 */

UCLASS(BlueprintType)
class UCharacterAnimSetterDataAsset : public UDataAsset
{
public:
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimSequenceBase* idlePoseAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimSequenceBase* aimPoseAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimSequenceBase* runLoopPoseAnim;
};
