// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"

#include "PlayerInputPollingSystem.h"
#include "InputQueueSystem.h"
#include "MainCharacterMovementComponent.h"
#include "Components\CapsuleComponent.h"

#include "EditorUtilities.h"

// Sets default values
AMainCharacter::AMainCharacter()
	: Super()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	playerInputPollingSystem = CreateDefaultSubobject<UPlayerInputPollingSystem>("PlayerInputPollingSystem");
	inputQueueSystem = CreateDefaultSubobject<UInputQueueSystem>("InputQueueSystem");
	mainCharacterMovementComponent = CreateDefaultSubobject<UMainCharacterMovementComponent>("MainCharacterMovementComponent");

	capsuleCollider = CreateDefaultSubobject<UCapsuleComponent>("CapsuleCollider");
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	playerInputPollingSystem->onAnInputTriggered.BindUObject(inputQueueSystem, &UInputQueueSystem::ConsumeInputs);

	mainCharacterMovementComponent->SetMoveableComponent(capsuleCollider);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector move = FVector::ZeroVector;

	if (SkeletalMeshComp1->IsPlayingRootMotion())
	{
		FRootMotionMovementParams RootMotion = SkeletalMeshComp1->ConsumeRootMotion();

		if (RootMotion.bHasRootMotion)
		{
			const FTransform WorldSpaceRootMotionTransform = SkeletalMeshComp1->ConvertLocalRootMotionToWorld(RootMotion.GetRootMotionTransform());
			FQuat NewRotation = WorldSpaceRootMotionTransform.GetRotation() * GetRootComponent()->GetComponentRotation().Quaternion();
			move = WorldSpaceRootMotionTransform.GetLocation();
		}
	}

	FQuat x = cameraBoom->GetComponentRotation().Quaternion() * FQuat::MakeFromEuler(FVector(0.f, lookInput.Y, 0.f));
	
	FQuat swing;
	FQuat twist;

	x.ToSwingTwist(FVector::RightVector, swing, twist);

	auto e = twist.Euler();

	e.Y = FMath::ClampAngle(e.Y, -70.f, 50.f);

	twist = FQuat::MakeFromEuler(e);

	cameraBoom->SetWorldRotation(swing * twist);

	FHitResult MoveOnBaseHit(1.f);
	//GetRootComponent()->MoveComponent(move, FQuat::MakeFromEuler(FVector(0.f, 0.f, lookInput.X)) * GetRootComponent()->GetComponentRotation().Quaternion(), true, &MoveOnBaseHit);

	mainCharacterMovementComponent->TryMoveByDelta(DeltaTime, move, FQuat::MakeFromEuler(FVector(0.f, 0.f, lookInput.X)) * GetRootComponent()->GetComponentRotation().Quaternion());
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

void AMainCharacter::SetHorizontalLookAxis(float value)
{
	lookInput.X = value * 10.f;
}

void AMainCharacter::SetVerticalLookAxis(float value)
{
	lookInput.Y = value * 10.f;
}

void AMainCharacter::HandleActionInput(EUserInputType inputType, EInputEvent inputEvent)
{
	playerInputPollingSystem->AddActionToUserInputPollingQueue(inputType, inputEvent);

	if (inputType == EUserInputType::JUMP_INPUT)
	{
		Cast<UPrimitiveComponent>(GetRootComponent())->AddForce(FVector::UpVector * 14000.f, NAME_None, true);
	}
}