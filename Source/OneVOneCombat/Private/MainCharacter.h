// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UserInputType.h"
#include "DataSubOwner.h"

#include "MainCharacter.generated.h"

class UDataTable;

class UPlayerInputPollingSystem;
class UInputQueueSystem;
class UMainCharacterDataAsset;
class UPlayerStateManager;
class UMainCharacterMovementComponent;
class UUserActionAndAxisInputHandler;
class UCharacterAnimInstance;
class UCharacterEvents;
class UStateEvents;
class UInventoryComponent;
class UCharacterAttributeDataAsset;

struct FCharacterInputData;
struct FAnimationRelatedData;
struct FCharacterStateData;

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
	inline UMainCharacterMovementComponent* GetMainMovementComponent() const { return movementComponent; }
	UFUNCTION(BlueprintPure)
	inline UPlayerStateManager* GetPlayerStateManager() const { return playerStateManager; }
	inline UInputQueueSystem* GetInputQueueSystem() const { return inputQueueSystem; }
	inline UPlayerInputPollingSystem* GetPlayerInputPollingSystem() const { return playerInputPollingSystem; }
	inline UCharacterAnimInstance* GetAnimInstance() const { return characterAnimInstance; }
	inline UCharacterEvents* GetCharacterEvents() const { return characterEvents; }
	inline UStateEvents* GetStateEvents() const { return stateEvents; }
 	inline const UDataTable* GetAnimationStateEventDataTable() const { return animationStateEventDataTable; }
 	inline const UCharacterAttributeDataAsset* GetCharacterAttributeDataAsset() const { return characterAttributeDataAsset; }
 	inline USkeletalMeshComponent* GetCharacterSkeletalMesh() const { return characterSkeletalMesh; }

	void HandleActionInput(EUserInputType inputType, EInputEvent inputEvent);

	UPROPERTY(BlueprintReadWrite)
	USceneComponent* cameraBoom;

	void PreRegisterAllComponents() override;

	// FIXME: For testing purposes
	void SpawnItemOnSocket(const FName& socketName, AActor* actor);

	float GetLastDeltaTime() const;

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

	UPROPERTY(VisibleAnywhere, BlueprintGetter = "GetPlayerStateManager")
	UPlayerStateManager* playerStateManager;

	UPROPERTY(VisibleAnywhere)
	UPlayerInputPollingSystem* playerInputPollingSystem;

	UPROPERTY(VisibleAnywhere)
	UInputQueueSystem* inputQueueSystem;

	UPROPERTY(EditAnywhere)
	UPrimitiveComponent* capsuleCollider;

	UPROPERTY(EditAnywhere)
	USceneComponent* verticalRotationComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* characterSkeletalMesh;

	UPROPERTY()
	UCharacterAnimInstance* characterAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMainCharacterDataAsset* data;

	UPROPERTY(VisibleAnywhere)
	UMainCharacterMovementComponent* movementComponent;

	UPROPERTY(EditAnywhere)
	UCharacterEvents* characterEvents;
	
	UPROPERTY(EditAnywhere)
	UStateEvents* stateEvents;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* animationStateEventDataTable;

	UPROPERTY(EditDefaultsOnly)
	UCharacterAttributeDataAsset* characterAttributeDataAsset;

	UPROPERTY(VisibleAnywhere)
	UInventoryComponent* inventoryComponent;

	UPROPERTY(VisibleAnywhere)
	AActor* lastAttachedItem;

	DataSubOwner<FCharacterInputData> inputData;
	DataSubOwner<FAnimationRelatedData> animationRelatedData;
	DataSubOwner<FCharacterStateData> characterStateData;

	float lastDeltaTime;

	void OnSprintDisableStateChanged(bool state);
};