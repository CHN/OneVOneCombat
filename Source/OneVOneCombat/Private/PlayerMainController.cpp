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

	horizontalMovementInputHandler = NewObject<UUserActionAndAxisInputHandler>();
	verticalMovementInputHandler = NewObject<UUserActionAndAxisInputHandler>();
}

void APlayerMainController::BeginPlay()
{
	Super::BeginPlay();

	auto* character = Cast<AMainCharacter>(GetPawn());

	check(character);

	horizontalMovementInputHandler->BindAxisFunction(character, &AMainCharacter::SetHorizontalMoveAxis);
	verticalMovementInputHandler->BindAxisFunction(character, &AMainCharacter::SetVerticalMoveAxis);

	InputComponent->BindAction(TEXT("Move_Left"), EInputEvent::IE_Pressed, horizontalMovementInputHandler, &UUserActionAndAxisInputHandler::HandleNegativeInputPressed);
	InputComponent->BindAction(TEXT("Move_Left"), EInputEvent::IE_Released, horizontalMovementInputHandler, &UUserActionAndAxisInputHandler::HandleNegativeInputReleased);
	InputComponent->BindAction(TEXT("Move_Right"), EInputEvent::IE_Pressed, horizontalMovementInputHandler, &UUserActionAndAxisInputHandler::HandlePositiveInputPressed);
	InputComponent->BindAction(TEXT("Move_Right"), EInputEvent::IE_Released, horizontalMovementInputHandler, &UUserActionAndAxisInputHandler::HandlePositiveInputReleased);

	InputComponent->BindAction(TEXT("Move_Backward"), EInputEvent::IE_Pressed, verticalMovementInputHandler, &UUserActionAndAxisInputHandler::HandleNegativeInputPressed);
	InputComponent->BindAction(TEXT("Move_Backward"), EInputEvent::IE_Released, verticalMovementInputHandler, &UUserActionAndAxisInputHandler::HandleNegativeInputReleased);
	InputComponent->BindAction(TEXT("Move_Forward"), EInputEvent::IE_Pressed, verticalMovementInputHandler, &UUserActionAndAxisInputHandler::HandlePositiveInputPressed);
	InputComponent->BindAction(TEXT("Move_Forward"), EInputEvent::IE_Released, verticalMovementInputHandler, &UUserActionAndAxisInputHandler::HandlePositiveInputReleased);

	InputComponent->BindAxis(TEXT("Move_Horizontal"), horizontalMovementInputHandler, &UUserActionAndAxisInputHandler::HandleAxisInput);
	InputComponent->BindAxis(TEXT("Move_Vertical"), verticalMovementInputHandler, &UUserActionAndAxisInputHandler::HandleAxisInput);

	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("Attack"), EInputEvent::IE_Pressed, character, &AMainCharacter::HandleActionInput, UserInputType::ATTACK_INPUT, EInputEvent::IE_Pressed);
	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("Attack"), EInputEvent::IE_Released, character, &AMainCharacter::HandleActionInput, UserInputType::ATTACK_INPUT, EInputEvent::IE_Released);

	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("Jump"), EInputEvent::IE_Pressed, character, &AMainCharacter::HandleActionInput, UserInputType::JUMP_INPUT, EInputEvent::IE_Pressed);
	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("Jump"), EInputEvent::IE_Released, character, &AMainCharacter::HandleActionInput, UserInputType::JUMP_INPUT, EInputEvent::IE_Released);

	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("Crouch"), EInputEvent::IE_Pressed, character, &AMainCharacter::HandleActionInput, UserInputType::CROUCH_INPUT, EInputEvent::IE_Pressed);
	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("Crouch"), EInputEvent::IE_Released, character, &AMainCharacter::HandleActionInput, UserInputType::CROUCH_INPUT, EInputEvent::IE_Released);
}

void APlayerMainController::HandleUserInput(FUserInput userInput)
{

}