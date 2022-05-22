// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataSubOwner.h"

/**
 * 
 */

template<typename DataType>
class ONEVONECOMBAT_API DataReadOwner : private DataSubOwner<DataType>
{
	template<typename DataType>
	friend class DataOwner;

public:

	inline const DataType* operator->() const
	{
		return data;
	}
};
