// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include <Animation/AnimClassInterface.h>
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */

class UDataTable;
class UCharacterEvents;
class UMainCharacterDataAsset;

UCLASS()
class UCharacterAnimInstance : public UAnimInstance
{	
	GENERATED_BODY()

private:

	void NativeInitializeAnimation() override;
	void BindAnimationEvents(const UDataTable* eventTable, TWeakObjectPtr<UCharacterEvents> characterEvents);

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe = "true", AllowPrivateAccess = "true"))
	UMainCharacterDataAsset* GetCharacterData() const
	{
		return characterData;
	}

	UPROPERTY(BlueprintGetter = "GetCharacterData", meta = (AllowPrivateAccess = "true"))
	UMainCharacterDataAsset* characterData;
};
