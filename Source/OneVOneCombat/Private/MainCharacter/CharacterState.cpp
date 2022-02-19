// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter/CharacterState.h"

#include "AnimationFlaggedBool.h"


UCharacterState::UCharacterState()
{
	jumpState = CreateDefaultSubobject<UAnimationFlaggedBool>("JumpState");
	swordAttackState = CreateDefaultSubobject<UAnimationFlaggedBool>("SwordAttackState");
}
