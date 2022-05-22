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

	movementComponent = CreateDefaultSubobject<UMainCharacterMovementComponent>("MovementComponent");

	verticalRotationComponent = CreateDefaultSubobject<USceneComponent>("VerticalRotation");
	verticalRotationComponent->AttachToComponent(capsuleCollider, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	movementComponent->SetVerticalRotationComponent(verticalRotationComponent);

	characterSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	characterSkeletalMesh->AttachToComponent(verticalRotationComponent, FAttachmentTransformRules::KeepRelativeTransform);

	characterEvents = CreateDefaultSubobject<UCharacterEvents>("CharacterEvents");
	stateEvents = CreateDefaultSubobject<UStateEvents>("StateEvents");

	inventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
}

void AMainCharacter::PreRegisterAllComponents()
{
	Super::PreRegisterAllComponents();
	data = DuplicateObject(data, this);
}

// FIXME: DELETE THIS
void AMainCharacter::SpawnItemOnSocket(const FName& socketName, AActor* actor)
{
	if (lastAttachedItem)
	{
		lastAttachedItem->SetActorHiddenInGame(true);
	}

	actor->AttachToComponent(characterSkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, socketName);
	actor->SetActorHiddenInGame(false);
	lastAttachedItem = actor;
}

float AMainCharacter::GetLastDeltaTime() const
{
	return lastDeltaTime;
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

	data->characterInputDataOwner.BeSubOwner(&inputData);
	data->animationRelatedDataOwner.BeSubOwner(&animationRelatedData);
	data->characterStateDataOwner.BeSubOwner(&characterStateData);

	characterAnimInstance = Cast<UCharacterAnimInstance>(characterSkeletalMesh->AnimScriptInstance);

	playerStateManager->Init(this);
	inventoryComponent->Init(this);

	characterEvents->onSprintDisableStateChanged.AddUObject(this, &AMainCharacter::OnSprintDisableStateChanged);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	lastDeltaTime = DeltaTime;

	ResetInputHandlerAccumulations();

	inputData->scaledMoveInput = inputData->rawMoveInput;
	inputData->scaledRotateInput = inputData->rawRotateInput * inputData->rotateSensitivity;

	if (characterSkeletalMesh->IsPlayingRootMotion())
	{
		FRootMotionMovementParams RootMotion = characterSkeletalMesh->ConsumeRootMotion();

		if (RootMotion.bHasRootMotion)
		{
			const FTransform WorldSpaceRootMotionTransform = characterSkeletalMesh->ConvertLocalRootMotionToWorld(RootMotion.GetRootMotionTransform());

			// FIXME: NewRotation never used
			animationRelatedData->rootMotionRotation = WorldSpaceRootMotionTransform.GetRotation() * GetRootComponent()->GetComponentRotation().Quaternion();
			animationRelatedData->rootMotionMoveDelta = WorldSpaceRootMotionTransform.GetLocation();
			animationRelatedData->isRootMotionBeingUsed = true;
		}
		else
		{
			animationRelatedData->rootMotionMoveDelta = FVector::ZeroVector;
			animationRelatedData->isRootMotionBeingUsed = false;
		}
	}
	else
	{
		animationRelatedData->rootMotionMoveDelta = FVector::ZeroVector;
		animationRelatedData->isRootMotionBeingUsed = false;
	}
}

void AMainCharacter::ResetInputHandlerAccumulations()
{
	horizontalMovementInputHandler->ResetAxisAccumulation();
	verticalMovementInputHandler->ResetAxisAccumulation();
	horizontalLookInputHandler->ResetAxisAccumulation();
	verticalLookInputHandler->ResetAxisAccumulation();
}

void AMainCharacter::OnSprintDisableStateChanged(bool state)
{
	characterStateData->isSprintDisabled = state;
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

	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("Sprint"), EInputEvent::IE_Released, this, &AMainCharacter::HandleActionInput, EUserInputType::SPRINT, EInputEvent::IE_Released);
	InputComponent->BindAction<FHandleActionInputDelegate>(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AMainCharacter::HandleActionInput, EUserInputType::SPRINT, EInputEvent::IE_Pressed);
}

void AMainCharacter::SetHorizontalMoveAxis(float value)
{
	inputData->rawMoveInput.X = value;
}

void AMainCharacter::SetVerticalMoveAxis(float value)
{
	inputData->rawMoveInput.Y = value;
}

void AMainCharacter::SetHorizontalLookAxis(float value)
{
	inputData->rawRotateInput.X = value;
}

void AMainCharacter::SetVerticalLookAxis(float value)
{
	inputData->rawRotateInput.Y = value;
}

void AMainCharacter::HandleActionInput(EUserInputType inputType, EInputEvent inputEvent)
{
	playerInputPollingSystem->AddActionToUserInputPollingQueue(inputType, inputEvent);
}