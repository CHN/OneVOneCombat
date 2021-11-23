// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMainController.h"

#include "UserInput.h"
#include "MainCharacter.h"
#include "PlayerInputPollingSystem.h"

// Sets default values
APlayerMainController::APlayerMainController()
	: Super()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	horizontalMovementInputHandler = CreateDefaultSubobject<UUserActionInputHandler>(TEXT("HorizontalAxisHandler"));
	verticalMovementInputHandler = CreateDefaultSubobject<UUserActionInputHandler>(TEXT("VerticalAxisHandler"));
}

// Called when the game starts or when spawned
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

// Called every frame
void APlayerMainController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerMainController::HandleUserInput(FUserInput userInput)
{

}