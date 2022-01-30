// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataSubOwner.h"

/**
 * 
 */

template<typename DataType>
class ONEVONECOMBAT_API DataInlineSubOwner : public DataSubOwner<DataType>
{
public:
	using DataSubOwner::data;

private:
	void* operator new(size_t);
	void* operator new(size_t, void*);
	void* operator new[](size_t);
	void* operator new[](size_t, void*);
};
