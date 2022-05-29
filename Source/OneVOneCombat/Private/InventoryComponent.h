// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataSubOwner.h"
#include "DataReadOwner.h"

#include "InventoryComponent.generated.h"

class AMainCharacter;

struct FInventoryData;
struct FCharacterStateData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	void Init(AMainCharacter* NewMainCharacter);

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	AMainCharacter* mainCharacter;

	void OnNextItemSelectInputTriggered();
	void OnPreviousItemSelectInputTriggered();

	void CreateStartingInventoryItems();

	DataSubOwner<FInventoryData> inventoryData;
	DataReadOwner<FCharacterStateData> characterStateData;

	UPROPERTY(EditDefaultsOnly, meta = (MustImplement = "InventoryItemInterface"))
	TArray<TSubclassOf<AActor>> startingItems;
};
