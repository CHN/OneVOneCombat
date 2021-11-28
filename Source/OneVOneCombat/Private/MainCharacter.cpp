// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"

#include "PlayerInputPollingSystem.h"
#include "InputQueueSystem.h"

#include "EditorUtilities.h"

// Sets default values
AMainCharacter::AMainCharacter()
	: Super()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	playerInputPollingSystem = NewObject<UPlayerInputPollingSystem>(this, "PlayerInputPollingSystem");
	inputQueueSystem = NewObject<UInputQueueSystem>(this, "InputQueueSystem");

	playerInputPollingSystem->onAnInputTriggered.BindUObject(inputQueueSystem, &UInputQueueSystem::ConsumeInputs);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorRightVector() * movementInput.X);
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * movementInput.Y);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMainCharacter::SetHorizontalMoveAxis(float value)
{
	movementInput.X = value;
}

void AMainCharacter::SetVerticalMoveAxis(float value)
{
	movementInput.Y = value;
}

void AMainCharacter::HandleActionInput(EUserInputType inputType, EInputEvent inputEvent)
{
	playerInputPollingSystem->AddActionToUserInputPollingQueue(inputType, inputEvent);
}
