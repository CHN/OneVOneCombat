// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"

#include "PlayerInputPollingSystem.h"
#include "InputQueueSystem.h"
#include "MainCharacterMovementComponent.h"
#include "PlayerStateManager.h"
#include "InventoryComponent.h"
#include "Components/CapsuleComponent.h"
#include "CharacterAnimInstance.h"
#include "CharacterEvents/CharacterEvents.h"
#include "CharacterEvents/StateEvents.h"

#include "MainCharacter/MainCharacterDataAsset.h"
#include "MainCharacter/CharacterInputData.h"
#include "MainCharacter/AnimationRelatedData.h"

#include "EditorUtilities.h"

// Sets default values
AMainCharacter::AMainCharacter()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	playerInputPollingSystem = CreateDefaultSubobject<UPlayerInputPollingSystem>("PlayerInputPollingSystem");
	inputQueueSystem = CreateDefaultSubobject<UInputQueueSystem>("InputQueueSystem");
	playerStateManager = CreateDefaultSubobject<UPlayerStateManager>("PlayerStateManager");

	capsuleCollider = CreateDefaultSubobject<UCapsuleComponent>("CapsuleCollider");
	characterSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	characterSkeletalMesh->AttachToComponent(capsuleCollider, FAttachmentTransformRules::KeepRelativeTransform);

	movementComponent = CreateDefaultSubobject<UMainCharacterMovementComponent>("MovementComponent");
	characterEvents = CreateDefaultSubobject<UCharacterEvents>("CharacterEvents");
	stateEvents = CreateDefaultSubobject<UStateEvents>("StateEvents");

	inventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
}

void AMainCharacter::PreRegisterAllComponents()
{
	Super::PreRegisterAllComponents();
	data = DuplicateObject(data, this);
}

void AMainCharacter::CreateInputHandlers()
{
	horizontalMovementInputHandler = NewObject<UUserActionAndAxisInputHandler>(this);
	verticalMovementInputHandler = NewObject<UUserActionAndAxisInputHandler>(this);
	horizontalLookInputHandler = NewObject<UUserActionAndAxisInputHandler>(this);
	verticalLookInputHandler = NewObject<UUserActionAndAxisInputHandler>(this);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{	
	Super::BeginPlay();

	playerInputPollingSystem->onAnInputTriggered.BindUObject(inputQueueSystem, &UInputQueueSystem::ConsumeInputs);

	movementComponent->SetMoveableComponent(capsuleCollider);

	data->characterInputDataOwner.BecomeSubOwner(&inputData);
	data->animationRelatedDataOwner.BecomeSubOwner(&animationRelatedData);

	characterAnimInstance = Cast<UCharacterAnimInstance>(characterSkeletalMesh->AnimScriptInstance);

	playerStateManager->Init(this);
	inventoryComponent->Init(this);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ResetInputHandlerAccumulations();

	if (characterSkeletalMesh->IsPlayingRootMotion())
	{
		FRootMotionMovementParams RootMotion = characterSkeletalMesh->ConsumeRootMotion();

		if (RootMotion.bHasRootMotion)
		{
			const FTransform WorldSpaceRootMotionTransform = characterSkeletalMesh->ConvertLocalRootMotionToWorld(RootMotion.GetRootMotionTransform());

			// FIXME: NewRotation never used
			animationRelatedData.data->rootMotionRotation = WorldSpaceRootMotionTransform.GetRotation() * GetRootComponent()->GetComponentRotation().Quaternion();
			animationRelatedData.data->rootMotionMoveDelta = WorldSpaceRootMotionTransform.GetLocation();
			animationRelatedData.data->isRootMotionBeingUsed = true;
		}
		else
		{
			animationRelatedData.data->rootMotionMoveDelta = FVector::ZeroVector;
			animationRelatedData.data->isRootMotionBeingUsed = false;
		}
	}
	else
	{
		animationRelatedData.data->rootMotionMoveDelta = FVector::ZeroVector;
		animationRelatedData.data->isRootMotionBeingUsed = false;
	}

	FQuat x = cameraBoom->GetComponentRotation().Quaternion() * FQuat::MakeFromEuler(FVector(0.f, inputData.data->rawRotateInput.Y, 0.f));
	
	FQuat swing;
	FQuat twist;

	x.ToSwingTwist(FVector::RightVector, swing, twist);

	auto e = twist.Euler();

	e.Y = FMath::ClampAngle(e.Y, -70.f, 50.f);

	twist = FQuat::MakeFromEuler(e);

	cameraBoom->SetWorldRotation(swing * twist);
}

void AMainCharacter::ResetInputHandlerAccumulations()
{
	horizontalMovementInputHandler->ResetAxisAccumulation();
	verticalMovementInputHandler->ResetAxisAccumulation();
	horizontalLookInputHandler->ResetAxisAccumulation();
	verticalLookInputHandler->ResetAxisAccumulation();
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	CreateInputHandlers();
	BindInputHandlerFunctions();
	BindMovementInputs();
	BindLookInputs();
	BindPlayerActionInputs();
}

void AMainCharacter::BindInputHandlerFunctions()
{
	horizontalMovementInputHandler->BindAxisFunction(this, &AMainCharacter::SetHorizontalMoveAxis);
	verticalMovementInputHandler->BindAxisFunction(this, &AMainCharacter::SetVerticalMoveAxis);

	horizontalLookInputHandler->BindAxisFunction(this, &AMainCharacter::SetHorizontalLookAxis);
	verticalLookInputHandler->BindAxisFunction(this, &AMainCharacter::SetVerticalLookAxis);
}

void AMainCharacter::BindMovementInputs()
{
	InputComponent->BindAction(TEXT("Move_Right"), EInputEvent::IE_Pressed, horizontalMovementInputHandler, &UUserActionAndAxisInputHandler::HandleNegativeInputPressed);
	InputComponent->BindAction(TEXT("Move_Right"), EInputEvent::IE_Released, horizontalMovementInputHandler, &UUserActionAndAxisInputHandler::HandleNegativeInputReleased);
	InputComponent->BindAction(TEXT("Move_Left"), EInputEvent::IE_Pressed, horizontalMovementInputHandler, &UUserActionAndAxisInputHandler::HandlePositiveInputPressed);
	InputComponent->BindAction(TEXT("Move_Left"), EInputEvent::IE_Released, horizontalMovementInputHandler, &UUserActionAndAxisInputHandler::HandlePositiveInputReleased);

	InputComponent->BindAction(TEXT("Move_Backward"), EInputEvent::IE_Pressed, verticalMovementInputHandler, &UUserActionAndAxisInputHandler::HandleNegativeInputPressed);
	InputComponent->BindAction(TEXT("Move_Backward"), EInputEvent::IE_Released, verticalMovementInputHandler, &UUserActionAndAxisInputHandler::HandleNegativeInputReleased);
	InputComponent->BindAction(TEXT("Move_Forward"), EInputEvent::IE_Pressed, verticalMovementInputHandler, &UUserActionAndAxisInputHandler::HandlePositiveInputPressed);
	InputComponent->BindAction(TEXT("Move_Forward"), EInputEvent::IE_Released, verticalMovementInputHandler, &UUserActionAndAxisInputHandler::HandlePositiveInputReleased);

	InputComponent->BindAxis(TEXT("Move_Horizontal"), horizontalMovementInputHandler, &UUserActionAndAxisInputHandler::HandleAxisInput);
	InputComponent->BindAxis(TEXT("Move_Vertical"), verticalMovementInputHandler, &UUserActionAndAxisInputHandler::HandleAxisInput);
}

void AMainCharacter::BindLookInputs()
{
	InputComponent->BindAxis(TEXT("Look_Horizontal"), horizontalLookInputHandler, &UUserActionAndAxisInputHandler::HandleAxisInput);
	InputComponent->BindAxis(TEXT("Look_Vertical"), verticalLookInputHandler, &UUserActionAndAxisInputHandler::HandleAxisInput);

	InputComponent->BindAxis(TEXT("Look_Horizontal_Joystick"), horizontalLookInputHandler, &UUserActionAndAxisInputHandler::HandleAxisInput);
	InputComponent->BindAxis(TEXT("Look_Vertical_Joystick"), verticalLookInputHandler, &UUserActionAndAxisInputHandler::HandleAxisInput);
}

void AMainCharacter::BindPlayerActionInputs()
{
	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMainCharacter::HandleActionInput, EUserInputType::ATTACK_INPUT, EInputEvent::IE_Pressed);
	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("Attack"), EInputEvent::IE_Released, this, &AMainCharacter::HandleActionInput, EUserInputType::ATTACK_INPUT, EInputEvent::IE_Released);

	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMainCharacter::HandleActionInput, EUserInputType::JUMP_INPUT, EInputEvent::IE_Pressed);
	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("Jump"), EInputEvent::IE_Released, this, &AMainCharacter::HandleActionInput, EUserInputType::JUMP_INPUT, EInputEvent::IE_Released);

	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &AMainCharacter::HandleActionInput, EUserInputType::CROUCH_INPUT, EInputEvent::IE_Pressed);
	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("Crouch"), EInputEvent::IE_Released, this, &AMainCharacter::HandleActionInput, EUserInputType::CROUCH_INPUT, EInputEvent::IE_Released);

	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("ChangeWeapon_Previous"), EInputEvent::IE_Pressed, this, &AMainCharacter::HandleActionInput, EUserInputType::CHANGE_WEAPON_PREVIOUS, EInputEvent::IE_Pressed);
	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("ChangeWeapon_Previous"), EInputEvent::IE_Released, this, &AMainCharacter::HandleActionInput, EUserInputType::CHANGE_WEAPON_PREVIOUS, EInputEvent::IE_Released);

	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("ChangeWeapon_Next"), EInputEvent::IE_Pressed, this, &AMainCharacter::HandleActionInput, EUserInputType::CHANGE_WEAPON_NEXT, EInputEvent::IE_Pressed);
	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("ChangeWeapon_Next"), EInputEvent::IE_Released, this, &AMainCharacter::HandleActionInput, EUserInputType::CHANGE_WEAPON_NEXT, EInputEvent::IE_Released);
}

void AMainCharacter::SetHorizontalMoveAxis(float value)
{
	inputData.data->rawMoveInput.X = value;
}

void AMainCharacter::SetVerticalMoveAxis(float value)
{
	inputData.data->rawMoveInput.Y = value;
}

void AMainCharacter::SetHorizontalLookAxis(float value)
{
	inputData.data->rawRotateInput.X = value * 10.f;
}

void AMainCharacter::SetVerticalLookAxis(float value)
{
	inputData.data->rawRotateInput.Y = value * 10.f;
}

void AMainCharacter::HandleActionInput(EUserInputType inputType, EInputEvent inputEvent)
{
	playerInputPollingSystem->AddActionToUserInputPollingQueue(inputType, inputEvent);
}