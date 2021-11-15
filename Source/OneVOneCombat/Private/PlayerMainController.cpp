// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMainController.h"

// Sets default values
APlayerMainController::APlayerMainController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerMainController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerMainController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerMainController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerMainController::HandleUserInput(FUserInput userInput)
{

}

void APlayerMainController::SetMovementInput(float xAxis, float yAxis)
{
	movementInput.X = xAxis;
	movementInput.Y = yAxis;
}