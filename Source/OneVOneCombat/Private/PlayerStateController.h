// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PlayerStateObject.h"

#include "PlayerStateController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(Ftestx);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPlayerStateController : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPlayerStateController();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AddPlayerState(UPlayerStateObject* newPlayerState);

	UFUNCTION(BlueprintCallable)
	bool TryToSwitchState(EPlayerState targetStateEnum);

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	Ftestx testx;

private:

	TArray<UPlayerStateObject*> playerStates;
	UPlayerStateObject* currentActiveState;

	void InitPlayerStates();
};
