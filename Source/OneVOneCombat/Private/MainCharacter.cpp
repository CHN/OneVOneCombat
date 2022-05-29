// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"

#include "MainCharacterPlayerState.h"
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

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{	
	Super::BeginPlay();

	mainCharacterPlayerState = Cast<AMainCharacterPlayerState>(APawn::GetPlayerState());

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

bool AMainCharacter::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
{
	if (Super::ProcessConsoleExec(Cmd, Ar, Executor))
	{
		return true;
	}

	if (inputQueueSystem->ProcessConsoleExec(Cmd, Ar, Executor))
	{
		return true;
	}

	return false;
}

void AMainCharacter::OnSprintDisableStateChanged(bool state)
{
	characterStateData->isSprintDisabled = state;
}