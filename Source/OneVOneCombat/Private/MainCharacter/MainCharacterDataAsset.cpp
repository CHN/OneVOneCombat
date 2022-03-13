// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter/MainCharacterDataAsset.h"

void UMainCharacterDataAsset::PostLoad()
{
	Super::PostLoad();
	
	movementComponentDataOwner = &movementComponentData;
	walkableGroundPropertiesDataOwner = &walkableGroundPropertiesData;
	characterInputDataOwner = &characterInputData;
	animationRelatedDataOwner = &animationRelatedData;
	characterStateDataOwner = &characterStateData;
	inventoryDataOwner = &inventoryData;
}
