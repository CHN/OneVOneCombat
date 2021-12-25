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

	horizontalMovementInputHandler = NewObject<UUserActionAndAxisInputHandler>();
	verticalMovementInputHandler = NewObject<UUserActionAndAxisInputHandler>();

	horizontalLookInputHandler = NewObject<UUserActionAndAxisInputHandler>();
	verticalLookInputHandler = NewObject<UUserActionAndAxisInputHandler>();
}

void APlayerMainController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	horizontalMovementInputHandler->ResetAxisAccumulation();
	verticalMovementInputHandler->ResetAxisAccumulation();
	horizontalLookInputHandler->ResetAxisAccumulation();
	verticalLookInputHandler->ResetAxisAccumulation();
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

	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("Attack"), EInputEvent::IE_Pressed, character, &AMainCharacter::HandleActionInput, EUserInputType::ATTACK_INPUT, EInputEvent::IE_Pressed);
	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("Attack"), EInputEvent::IE_Released, character, &AMainCharacter::HandleActionInput, EUserInputType::ATTACK_INPUT, EInputEvent::IE_Released);

	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("Jump"), EInputEvent::IE_Pressed, character, &AMainCharacter::HandleActionInput, EUserInputType::JUMP_INPUT, EInputEvent::IE_Pressed);
	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("Jump"), EInputEvent::IE_Released, character, &AMainCharacter::HandleActionInput, EUserInputType::JUMP_INPUT, EInputEvent::IE_Released);

	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("Crouch"), EInputEvent::IE_Pressed, character, &AMainCharacter::HandleActionInput, EUserInputType::CROUCH_INPUT, EInputEvent::IE_Pressed);
	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("Crouch"), EInputEvent::IE_Released, character, &AMainCharacter::HandleActionInput, EUserInputType::CROUCH_INPUT, EInputEvent::IE_Released);

	// Handle Look Input

	horizontalLookInputHandler->BindAxisFunction(character, &AMainCharacter::SetHorizontalLookAxis);
	verticalLookInputHandler->BindAxisFunction(character, &AMainCharacter::SetVerticalLookAxis);

	InputComponent->BindAxis(TEXT("Look_Horizontal"), horizontalLookInputHandler, &UUserActionAndAxisInputHandler::HandleAxisInput);
	InputComponent->BindAxis(TEXT("Look_Vertical"), verticalLookInputHandler, &UUserActionAndAxisInputHandler::HandleAxisInput);

	InputComponent->BindAxis(TEXT("Look_Horizontal_Joystick"), horizontalLookInputHandler, &UUserActionAndAxisInputHandler::HandleAxisInput);
	InputComponent->BindAxis(TEXT("Look_Vertical_Joystick"), verticalLookInputHandler, &UUserActionAndAxisInputHandler::HandleAxisInput);
}