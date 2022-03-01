// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStateGroupBase.h"
#include "DefaultStateGroup.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UDefaultStateGroup : public UPlayerStateGroupBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDefaultStateGroup();
public:
	void OnInitialized() override;

};
