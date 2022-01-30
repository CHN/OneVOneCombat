// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "UserInputType.h"

#include "MainCharacter.generated.h"

class UPlayerInputPollingSystem;
class UInputQueueSystem;
class UMainCharacterMovementComponent;
class UMainCharacterData;

enum class EUserInputType : uint8;

DECLARE_DELEGATE_TwoParams(FHandleActionInputDelegate, EUserInputType, EInputEvent);

UCLASS(Blueprintable)
class AMainCharacter : public APawn
{
	GENERATED_BODY()

public:

	AMainCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetHorizontalMoveAxis(float value);
	void SetVerticalMoveAxis(float value);

	void SetHorizontalLookAxis(float value);
	void SetVerticalLookAxis(float value);

	inline UMainCharacterData* const GetCharacterData() const { return data; }

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector2D movementInput;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector2D lookInput;

	void HandleActionInput(EUserInputType inputType, EInputEvent inputEvent);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMeshComp1;

	UPROPERTY(BlueprintReadWrite)
	USceneComponent* cameraBoom;

protected:
	 
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere)
	UPlayerInputPollingSystem* playerInputPollingSystem;

	UPROPERTY(VisibleAnywhere)
	UInputQueueSystem* inputQueueSystem;

	UPROPERTY(EditAnywhere)
	UPrimitiveComponent* capsuleCollider;

	UPROPERTY(VisibleAnywhere)
	UMainCharacterMovementComponent* mainCharacterMovementComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UMainCharacterData* data;
};