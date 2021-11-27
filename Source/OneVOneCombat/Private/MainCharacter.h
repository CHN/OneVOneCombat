// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "PlayerStateController.h"

#include "MainCharacter.generated.h"

class UPlayerInputPollingSystem;
class UInputQueueSystem;

UCLASS(Blueprintable)
class AMainCharacter : public APawn
{
	GENERATED_BODY()

public:

	AMainCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetHorizontalMoveAxis(float value);
	void SetVerticalMoveAxis(float value);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector2D movementInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPlayerStateController* playerStateController;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	UPlayerInputPollingSystem* playerInputPollingSystem;
	UInputQueueSystem* inputQueueSystem;
};
