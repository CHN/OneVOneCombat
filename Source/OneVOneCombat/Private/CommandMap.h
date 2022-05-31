// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CommandMap.generated.h"

/**
 * 
 */

DECLARE_EVENT(UCommandMap, FCommandExecuteEvent);

USTRUCT()
struct FCommandData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	uint8 blockerCount;
	FCommandExecuteEvent commandEvent;
};

UCLASS()
class UCommandMap : public UObject
{
	GENERATED_BODY()
public:

	template<typename UserClass, typename ...VarTypes>
	FDelegateHandle BindCommand(const FString& command, UserClass* object, typename FCommandExecuteEvent::FDelegate::TMethodPtr<UserClass, VarTypes...> function, VarTypes... vars)
	{
		FCommandExecuteEvent& executeEvent = commandDataMap.FindOrAdd(command).commandEvent;
		return executeEvent.AddUObject(object, function, vars...);
	}

	void RemoveCommand(const FString& command, const FDelegateHandle& delegateHandle);

	UFUNCTION(BlueprintCallable)
	void BlockCommand(const FString& cmd);

	UFUNCTION(BlueprintCallable)
	void UnblockCommand(const FString& cmd);

	UFUNCTION(BlueprintCallable)
	bool InvokeCommand(const FString& command);

private:

	TMap<FString, FCommandData> commandDataMap;
};
