// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

#define LOG_TO_SCREEN(text, ...) GEngine->AddOnScreenDebugMessage(rand(), 0.5f, FColor::Cyan, FString::Printf(TEXT(text), __VA_ARGS__))
#define LOG_TO_SCREEN_STR(text, ...) GEngine->AddOnScreenDebugMessage(rand(), 0.5f, FColor::Cyan, FString::Format(TEXT(text), { __VA_ARGS__ }))

class EditorUtilities
{
public:

	static FString EnumToString(const TCHAR* Enum, int32 EnumValue)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, Enum, true);
		if (!EnumPtr)
			return NSLOCTEXT("Invalid", "Invalid", "Invalid").ToString();

#if WITH_EDITOR
		return EnumPtr->GetDisplayNameTextByIndex(EnumValue).ToString();
#else
		return EnumPtr->GetEnumName(EnumValue);
#endif
	}
};
