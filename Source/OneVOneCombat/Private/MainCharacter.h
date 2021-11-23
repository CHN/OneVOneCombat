// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "PlayerStateController.h"

#include "MainCharacter.generated.h"

UCLASS(Blueprintable)
class AMainCharacter : public APawn
{
	GENERATED_BODY()

public:

	AMainCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetHorizontalMoveAxis(float value);
	void SetVerticalMoveAxis(float value);

	UPROPERTY(BlueprintReadOnly)
	FVector2D movementInput;
protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere)
	UPlayerStateController* playerStateController;
};
