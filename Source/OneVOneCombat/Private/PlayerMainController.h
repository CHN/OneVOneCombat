// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "UserInput.h"
#include "PlayerStateObject.h"

#include "PlayerMainController.generated.h"

UCLASS()
class APlayerMainController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerMainController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void HandleUserInput(FUserInput userInput);

	UFUNCTION(BlueprintCallable)
	void SetMovementInput(float xAxis, float yAxis);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	FVector2D movementInput;
};
