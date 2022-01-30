// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

template<typename DataType>
class DataOwner;

/**
 * 
 */

template<typename DataType>
class ONEVONECOMBAT_API DataSubOwner
{
public:
	DataSubOwner() {}
	~DataSubOwner();

	void OnDataOwnerDestroyed();

	void OnDataOwnerEndSubOwnership()
	{
		ResetSubOwner(-1, nullptr, nullptr);
	}

	void ResetSubOwner(int32 newId, DataType* newData, DataOwner<DataType>* newDataOwner)
	{
		id = newId;
		data = newData;
		dataOwner = newDataOwner;
	}

	int32 GetId() { return id; }

private:
	int32 id = -1;

protected:
	DataType* data;

private:
	DataOwner<DataType>* dataOwner;
};

#include "DataOwner.h"

template<typename DataType>
void DataSubOwner<DataType>::OnDataOwnerDestroyed()
{
	id = -1;
	data = nullptr;
	dataOwner = nullptr;
}


template<typename DataType>
DataSubOwner<DataType>::~DataSubOwner()
{
	if (dataOwner)
	{
		dataOwner->OnSubOwnerDestroyed(this);
	}
}