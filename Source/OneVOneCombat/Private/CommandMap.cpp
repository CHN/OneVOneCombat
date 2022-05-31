// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandMap.h"

bool UCommandMap::InvokeCommand(const FString& command)
{
	auto* commandData = commandDataMap.Find(command);

	if (commandData && commandData->blockerCount == 0)
	{
		commandData->commandEvent.Broadcast();

		return true;
	}

	return false;
}

void UCommandMap::RemoveCommand(const FString& command, const FDelegateHandle& delegateHandle)
{
	commandDataMap[command].commandEvent.Remove(delegateHandle);
}

void UCommandMap::BlockCommand(const FString& cmd)
{
	FCommandData* commandData = commandDataMap.Find(cmd);

	if (commandData)
	{
		++commandData->blockerCount;
	}
}

void UCommandMap::UnblockCommand(const FString& cmd)
{
	FCommandData* commandData = commandDataMap.Find(cmd);

	if (commandData && commandData->blockerCount > 0)
	{
		--commandData->blockerCount;
	}
}