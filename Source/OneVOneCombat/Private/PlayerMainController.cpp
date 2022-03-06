// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMainController.h"

#include "UserInputType.h"
#include "MainCharacter.h"

// Sets default values
APlayerMainController::APlayerMainController()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerMainController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void APlayerMainController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void APlayerMainController::BeginPlay()
{
	Super::BeginPlay();
}