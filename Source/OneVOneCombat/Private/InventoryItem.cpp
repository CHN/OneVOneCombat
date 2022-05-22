// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"

void UInventoryItem::BeginPlay()
{
	Super::BeginPlay();
	
	extraData = DuplicateObject<UDataAsset>(extraData, this);
}
