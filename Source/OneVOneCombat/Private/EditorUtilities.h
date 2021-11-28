// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

#define LOG_TO_SCREEN(text, ...) GEngine->AddOnScreenDebugMessage(rand(), 0.5f, FColor::Cyan, FString::Printf(TEXT(text), __VA_ARGS__))

class EditorUtilities
{
public:

};
