// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

#if WITH_EDITOR

#define LOG_TO_SCREEN(text, ...) GEngine->AddOnScreenDebugMessage(rand(), 0.5f, FColor::Cyan, FString::Format(TEXT(text), FStringFormatOrderedArguments({__VA_ARGS__})))

#define LOG_TO_SCREEN_FMT(text, ...) GEngine->AddOnScreenDebugMessage(rand(), 0.5f, FColor::Cyan, FString::Printf(TEXT(text), __VA_ARGS__))

#define DEBUG_CAPSULE(primitive) DrawDebugCapsule(GetWorld(), primitive->GetComponentLocation(), primitive->GetCollisionShape().GetCapsuleHalfHeight(), primitive->GetCollisionShape().GetCapsuleRadius(), primitive->GetComponentRotation().Quaternion(), FColor::Red)

#define DEBUG_CAPSULE_COLOR(primitive, color) DrawDebugCapsule(GetWorld(), primitive->GetComponentLocation(), primitive->GetCollisionShape().GetCapsuleHalfHeight(), primitive->GetCollisionShape().GetCapsuleRadius(), primitive->GetComponentRotation().Quaternion(), color)

#define DEBUG_ARROW(origin, end) DrawDebugDirectionalArrow(GetWorld(), origin, origin + end, 45.f, FColor::Blue)

#define DEBUG_ARROW_COLOR(origin, end, color) DrawDebugDirectionalArrow(GetWorld(), origin, origin + end, 45.f, color);

#define DEBUG_ARROW_DISP(origin, displacement) DrawDebugDirectionalArrow(GetWorld(), origin, origin + displacement, 45.f, FColor::Blue)

#define DEBUG_ARROW_DISP_COLOR(origin, displacement, color) DrawDebugDirectionalArrow(GetWorld(), origin, origin + displacement, 45.f, color)

#else
	
#define LOG_TO_SCREEN(...)
#define LOG_TO_SCREEN_FMT(...)

#define DEBUG_CAPSULE(...)
#define DEBUG_CAPSULE_COLOR(...)
#define DEBUG_ARROW(...)
#define DEBUG_ARROW_COLOR(...)
#define DEBUG_ARROW_DISP(...)
#define DEBUG_ARROW_DISP_COLOR(...)

#endif

class EditorUtilities
{
public:

	template<typename EnumType>
	static FString EnumToString(const TCHAR* Enum, EnumType EnumValue)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, Enum, true);
		if (!EnumPtr)
			return NSLOCTEXT("Invalid", "Invalid", "Invalid").ToString();

#if WITH_EDITOR
		return EnumPtr->GetDisplayNameTextByIndex(static_cast<int32>(EnumValue)).ToString();
#else
		return EnumPtr->GetEnumName(static_cast<int32>(EnumValue));
#endif
	}
};