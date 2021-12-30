// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MainCharacterMovementComponent.generated.h"

USTRUCT()
struct FWalkableGroundProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float minSlopeDotProduct = 0.95f;

	UPROPERTY(EditAnywhere)
	float maxSlopeDotProduct = 1.f;

	UPROPERTY(VisibleAnywhere)
	float sweepGroundAvoidanceDistance = 1.f;

	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<ECollisionChannel> collisionChannel;
};

USTRUCT()
struct FCharacterMovementProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector gravityConstant = FVector(0.f, 0.f, -980.f);
	
	UPROPERTY(EditAnywhere)
	FVector currentVelocity;
};

UCLASS(Blueprintable)
class UMainCharacterMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UMainCharacterMovementComponent();

	UFUNCTION(BlueprintCallable)
	void SetMoveableComponent(UPrimitiveComponent* NewMoveableComponent);

	bool TryMoveByDelta(const float deltaTime, const FVector& delta, const FQuat& worldRotation);

private:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FWalkableGroundProperties walkableGroundProperties;

	UPROPERTY(EditAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> sweepCollisionChannel;

	UPROPERTY(EditAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FCharacterMovementProperties movementProperties;

	TObjectPtr<UPrimitiveComponent> moveableComponent;
};
