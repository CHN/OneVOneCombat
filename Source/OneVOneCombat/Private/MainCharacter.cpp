// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"

#include "PlayerInputPollingSystem.h"
#include "InputQueueSystem.h"
#include "MainCharacterMovementComponent.h"
#include "PlayerStateManager.h"
#include "Components/CapsuleComponent.h"
#include "MainCharacter/MainCharacterComponentGroup.h"

#include "MainCharacter/MainCharacterData.h"

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
	componentGroup = CreateDefaultSubobject<UMainCharacterComponentGroup>("ComponentGroup");
	playerStateManager = CreateDefaultSubobject<UPlayerStateManager>("PlayerStateManager");

	capsuleCollider = CreateDefaultSubobject<UCapsuleComponent>("CapsuleCollider");

	data = CreateDefaultSubobject<UMainCharacterData>("MainCharacterData");
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	playerInputPollingSystem->onAnInputTriggered.BindUObject(inputQueueSystem, &UInputQueueSystem::ConsumeInputs);

	auto movementComponent = componentGroup->GetMovementComponent();

	playerStateManager->Init(data, componentGroup);

	inputQueueSystem->inputQueueSystemEvent.BindUObject(playerStateManager, &UPlayerStateManager::OnInputQueueOutputStateTriggered);

	movementComponent->SetMoveableComponent(capsuleCollider);

	data->characterInputDataOwner.BecomeSubOwner(&inputData);
	data->animationRelatedDataOwner.BecomeSubOwner(&animationRelatedData);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SkeletalMeshComp1->IsPlayingRootMotion())
	{
		FRootMotionMovementParams RootMotion = SkeletalMeshComp1->ConsumeRootMotion();

		if (RootMotion.bHasRootMotion)
		{
			const FTransform WorldSpaceRootMotionTransform = SkeletalMeshComp1->ConvertLocalRootMotionToWorld(RootMotion.GetRootMotionTransform());
			FQuat NewRotation = WorldSpaceRootMotionTransform.GetRotation() * GetRootComponent()->GetComponentRotation().Quaternion();
			animationRelatedData.data->rootMotionMoveDelta = WorldSpaceRootMotionTransform.GetLocation();
			animationRelatedData.data->isRootMotionBeingUsed = true;
		}
		else
		{
			animationRelatedData.data->isRootMotionBeingUsed = false;
		}
	}
	else
	{
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

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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