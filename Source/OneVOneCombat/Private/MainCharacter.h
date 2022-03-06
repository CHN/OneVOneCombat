// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "UserInputType.h"
#include "DataInlineSubOwner.h"

#include "MainCharacter.generated.h"

class UPlayerInputPollingSystem;
class UInputQueueSystem;
class UMainCharacterDataAsset;
class UCharacterState;
class UPlayerStateManager;
class UMainCharacterMovementComponent;

struct FCharacterInputData;
struct FAnimationRelatedData;

enum class EUserInputType : uint8;

DECLARE_DELEGATE_TwoParams(FHandleActionInputDelegate, EUserInputType, EInputEvent);

UCLASS(BlueprintType, Blueprintable)
class AMainCharacter : public APawn
{
	GENERATED_BODY()

public:

	AMainCharacter();

	void CreateInputHandlers();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void BindInputHandlerFunctions();
	void BindMovementInputs();
	void BindLookInputs();
	void BindPlayerActionInputs();

	void SetHorizontalMoveAxis(float value);
	void SetVerticalMoveAxis(float value);

	void SetHorizontalLookAxis(float value);
	void SetVerticalLookAxis(float value);

	inline UMainCharacterDataAsset* GetCharacterData() const { return data; }
	inline UCharacterState* GetCharacterState() const { return characterState; }
	inline UMainCharacterMovementComponent* GetMainMovementComponent() const { return movementComponent; }
	inline UPlayerStateManager* GetPlayerStateManager() const { return playerStateManager; }
	inline UInputQueueSystem* GetInputQueueSystem() const { return inputQueueSystem; }

	void HandleActionInput(EUserInputType inputType, EInputEvent inputEvent);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMeshComp1;

	UPROPERTY(BlueprintReadWrite)
	USceneComponent* cameraBoom;

	void PreRegisterAllComponents() override;

protected:
	 
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void ResetInputHandlerAccumulations();

private:

	UPROPERTY()
	UUserActionAndAxisInputHandler* horizontalMovementInputHandler;

	UPROPERTY()
	UUserActionAndAxisInputHandler* verticalMovementInputHandler;

	UPROPERTY()
	UUserActionAndAxisInputHandler* horizontalLookInputHandler;

	UPROPERTY()
	UUserActionAndAxisInputHandler* verticalLookInputHandler;

	UPROPERTY(VisibleAnywhere)
	UPlayerStateManager* playerStateManager;

	UPROPERTY(VisibleAnywhere)
	UPlayerInputPollingSystem* playerInputPollingSystem;

	UPROPERTY(VisibleAnywhere)
	UInputQueueSystem* inputQueueSystem;

	UPROPERTY(EditAnywhere)
	UPrimitiveComponent* capsuleCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMainCharacterDataAsset* data;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCharacterState* characterState;

	UPROPERTY(VisibleAnywhere)
	UMainCharacterMovementComponent* movementComponent;

	DataInlineSubOwner<FCharacterInputData> inputData;
	DataInlineSubOwner<FAnimationRelatedData> animationRelatedData;
};