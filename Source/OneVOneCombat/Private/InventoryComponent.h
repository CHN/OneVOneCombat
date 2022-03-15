// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataInlineSubOwner.h"

#include "InventoryComponent.generated.h"

class AMainCharacter;
class IInventoryItemInterface;

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

	void OnNextItemSelectInputTriggered(EInputEvent inputEvent);
	void OnPreviousItemSelectInputTriggered(EInputEvent inputEvent);

	void CreateStartingInventoryItems();

	DataInlineSubOwner<FInventoryData> inventoryData;
	DataInlineSubOwner<FCharacterStateData> characterStateData;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AActor>> startingItems;
};
