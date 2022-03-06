// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserActionAndAxisInputHandler.h"
#include "GameFramework\PlayerController.h"

#include "PlayerMainController.generated.h"

UCLASS()
class APlayerMainController : public APlayerController
{
	GENERATED_BODY()

public:

	APlayerMainController();

	void PreInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
};
