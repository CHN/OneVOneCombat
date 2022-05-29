// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework\PlayerController.h"

#include "PlayerMainController.generated.h"

class AMainCharacterPlayerState;
class UUserActionAndAxisInputHandler;

UCLASS()
class APlayerMainController : public APlayerController
{
	GENERATED_BODY()

private:

	virtual void PlayerTick(float DeltaTime) override;
	void InitPlayerState() override;
	void SetupInputComponent() override;

	UPROPERTY()
	UUserActionAndAxisInputHandler* moveInputHandler;

	UPROPERTY()
	UUserActionAndAxisInputHandler* lookInputHandler;

	TObjectPtr<AMainCharacterPlayerState> characterPlayerState;

	void InitMoveInput();
	void InitLookInput();

	void OnMoveHandled(FVector value);
	void OnLookHandled(FVector value);

	void ExecCommand(const TCHAR* cmd);
};
