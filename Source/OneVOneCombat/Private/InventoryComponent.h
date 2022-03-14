// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AMainCharacter;

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

	void OnSelectedItemSwitchedToNext(EInputEvent inputEvent);
	void OnSelectedItemSwitchedToPrevious(EInputEvent inputEvent);
};
