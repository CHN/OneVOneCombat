// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInput.h"

FUserInput::FUserInput(const FString& command)
{
	this->command = command;
	timeStamp = FDateTime::Now();
}
