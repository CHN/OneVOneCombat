// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMainController.h"
#include "MainCharacterPlayerState.h"
#include "UserActionAndAxisInputHandler.h"

#include "EditorUtilities.h"

DECLARE_DELEGATE_OneParam(FCommandExecuteDelegate, const TCHAR*);
#define BIND_COMMAND_INPUT(InputCmd, InputEvent, Cmd) InputComponent->BindAction<FCommandExecuteDelegate, APlayerMainController, const TCHAR*>(InputCmd, InputEvent, this, &APlayerMainController::ExecCommand, Cmd)

void APlayerMainController::InitPlayerState()
{
	Super::InitPlayerState();

	characterPlayerState = Cast<AMainCharacterPlayerState>(PlayerState);
}

void APlayerMainController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InitMoveInput();
	InitLookInput();

	BIND_COMMAND_INPUT("Jump", EInputEvent::IE_Pressed, TEXT("+jump"));
	BIND_COMMAND_INPUT("Jump", EInputEvent::IE_Released, TEXT("-jump"));
	BIND_COMMAND_INPUT("Interact", EInputEvent::IE_Pressed, TEXT("+interact"));
	BIND_COMMAND_INPUT("Interact", EInputEvent::IE_Released, TEXT("-interact"));
	BIND_COMMAND_INPUT("UsePrimaryWeapon", EInputEvent::IE_Pressed, TEXT("+usePrimaryWeapon"));
	BIND_COMMAND_INPUT("UsePrimaryWeapon", EInputEvent::IE_Released, TEXT("-usePrimaryWeapon"));
	BIND_COMMAND_INPUT("UseSecondaryWeapon", EInputEvent::IE_Pressed, TEXT("+useSecondaryWeapon"));
	BIND_COMMAND_INPUT("UseSecondaryWeapon", EInputEvent::IE_Released, TEXT("-useSecondaryWeapon"));
	BIND_COMMAND_INPUT("Sprint", EInputEvent::IE_Pressed, TEXT("+sprint"));
	BIND_COMMAND_INPUT("Sprint", EInputEvent::IE_Released, TEXT("-sprint"));
	BIND_COMMAND_INPUT("ChangeWeaponPrevious", EInputEvent::IE_Pressed, TEXT("+weaponPrev"));
	BIND_COMMAND_INPUT("ChangeWeaponPrevious", EInputEvent::IE_Released, TEXT("-weaponPrev"));
	BIND_COMMAND_INPUT("ChangeWeaponNext", EInputEvent::IE_Pressed, TEXT("+weaponNext"));
	BIND_COMMAND_INPUT("ChangeWeaponNext", EInputEvent::IE_Released, TEXT("-weaponNext"));
}

void APlayerMainController::PlayerTick(float DeltaTime)
{
	moveInputHandler->ResetAxisAccumulation();
	lookInputHandler->ResetAxisAccumulation();

	Super::PlayerTick(DeltaTime);
}

void APlayerMainController::InitMoveInput()
{
	moveInputHandler = NewObject<UUserActionAndAxisInputHandler>(this);

	InputComponent->BindAction("MoveLeft", EInputEvent::IE_Pressed, moveInputHandler, &UUserActionAndAxisInputHandler::HandleXAxisNegativeInputPressed);
	InputComponent->BindAction("MoveLeft", EInputEvent::IE_Released, moveInputHandler, &UUserActionAndAxisInputHandler::HandleXAxisNegativeInputReleased);

	InputComponent->BindAction("MoveRight", EInputEvent::IE_Pressed, moveInputHandler, &UUserActionAndAxisInputHandler::HandleXAxisPositiveInputPressed);
	InputComponent->BindAction("MoveRight", EInputEvent::IE_Released, moveInputHandler, &UUserActionAndAxisInputHandler::HandleXAxisPositiveInputReleased);

	InputComponent->BindAction("MoveForward", EInputEvent::IE_Pressed, moveInputHandler, &UUserActionAndAxisInputHandler::HandleYAxisNegativeInputPressed);
	InputComponent->BindAction("MoveForward", EInputEvent::IE_Released, moveInputHandler, &UUserActionAndAxisInputHandler::HandleYAxisNegativeInputReleased);

	InputComponent->BindAction("MoveBackward", EInputEvent::IE_Pressed, moveInputHandler, &UUserActionAndAxisInputHandler::HandleYAxisPositiveInputPressed);
	InputComponent->BindAction("MoveBackward", EInputEvent::IE_Released, moveInputHandler, &UUserActionAndAxisInputHandler::HandleYAxisPositiveInputReleased);

	InputComponent->BindAxis("Joystick_MoveX", moveInputHandler, &UUserActionAndAxisInputHandler::HandleXAxisInput);
	InputComponent->BindAxis("Joystick_MoveY", moveInputHandler, &UUserActionAndAxisInputHandler::HandleYAxisInput);

	moveInputHandler->BindAxisFunction(this, &APlayerMainController::OnMoveHandled);
}

void APlayerMainController::InitLookInput()
{
	lookInputHandler = NewObject<UUserActionAndAxisInputHandler>(this);

	InputComponent->BindAxis("Axis_LookX", lookInputHandler, &UUserActionAndAxisInputHandler::HandleXAxisInput);
	InputComponent->BindAxis("Axis_LookY", lookInputHandler, &UUserActionAndAxisInputHandler::HandleYAxisInput);

	lookInputHandler->BindAxisFunction(this, &APlayerMainController::OnLookHandled);
}

void APlayerMainController::OnMoveHandled(FVector value)
{
	if (characterPlayerState)
	{
		characterPlayerState->rawMoveInput = value;
	}
}

void APlayerMainController::OnLookHandled(FVector value)
{
	if (characterPlayerState)
	{
		characterPlayerState->rawLookInput = value;
	}
}

void APlayerMainController::ExecCommand(const TCHAR* cmd)
{
	APawn* pawn = GetPawn();

	if (pawn)
	{
		pawn->ProcessConsoleExec(cmd, *GLog, this);
	}
}