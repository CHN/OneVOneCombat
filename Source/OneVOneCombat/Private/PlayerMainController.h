// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "UserActionInputHandler.h"
#include "PlayerStateObject.h"
#include "MainCharacter.h"

#include "PlayerMainController.generated.h"

class UPlayerComboSystem;
class UPlayerInputPollingSystem;


UCLASS()
class APlayerMainController : public APlayerController
{
	GENERATED_BODY()

public:

	APlayerMainController();

	void PreInitializeComponents() override;

	UFUNCTION(BlueprintCallable)
	void HandleUserInput(FUserInput userInput);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	UUserActionInputHandler* horizontalMovementInputHandler;

	UPROPERTY(EditAnywhere)
	UUserActionInputHandler* verticalMovementInputHandler;
};
