// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMainController.h"

#include "UserInput.h"
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

	horizontalMovementInputHandler = NewObject<UUserActionInputHandler>();
	verticalMovementInputHandler = NewObject<UUserActionInputHandler>();
}

void APlayerMainController::BeginPlay()
{
	Super::BeginPlay();

	auto* character = Cast<AMainCharacter>(GetPawn());

	check(character);

	horizontalMovementInputHandler->BindAxisFunction(character, &AMainCharacter::SetHorizontalMoveAxis);
	verticalMovementInputHandler->BindAxisFunction(character, &AMainCharacter::SetVerticalMoveAxis);

	InputComponent->BindAction(TEXT("Move_Left"), EInputEvent::IE_Pressed, horizontalMovementInputHandler, &UUserActionInputHandler::HandleNegativeInputPressed);
	InputComponent->BindAction(TEXT("Move_Left"), EInputEvent::IE_Released, horizontalMovementInputHandler, &UUserActionInputHandler::HandleNegativeInputReleased);
	InputComponent->BindAction(TEXT("Move_Right"), EInputEvent::IE_Pressed, horizontalMovementInputHandler, &UUserActionInputHandler::HandlePositiveInputPressed);
	InputComponent->BindAction(TEXT("Move_Right"), EInputEvent::IE_Released, horizontalMovementInputHandler, &UUserActionInputHandler::HandlePositiveInputReleased);

	InputComponent->BindAction(TEXT("Move_Backward"), EInputEvent::IE_Pressed, verticalMovementInputHandler, &UUserActionInputHandler::HandleNegativeInputPressed);
	InputComponent->BindAction(TEXT("Move_Backward"), EInputEvent::IE_Released, verticalMovementInputHandler, &UUserActionInputHandler::HandleNegativeInputReleased);
	InputComponent->BindAction(TEXT("Move_Forward"), EInputEvent::IE_Pressed, verticalMovementInputHandler, &UUserActionInputHandler::HandlePositiveInputPressed);
	InputComponent->BindAction(TEXT("Move_Forward"), EInputEvent::IE_Released, verticalMovementInputHandler, &UUserActionInputHandler::HandlePositiveInputReleased);

	InputComponent->BindAxis(TEXT("Move_Horizontal"), horizontalMovementInputHandler, &UUserActionInputHandler::HandleAxisInput);
	InputComponent->BindAxis(TEXT("Move_Vertical"), verticalMovementInputHandler, &UUserActionInputHandler::HandleAxisInput);
}

void APlayerMainController::HandleUserInput(FUserInput userInput)
{

}