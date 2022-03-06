// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter/MainCharacterDataAsset.h"

UMainCharacterDataAsset::UMainCharacterDataAsset()
	: Super(), 
	movementComponentDataOwner(&movementComponentData),
	walkableGroundPropertiesDataOwner(&walkableGroundPropertiesData),
	characterInputDataOwner(&characterInputData),
	animationRelatedDataOwner(&animationRelatedData)
{
	
}