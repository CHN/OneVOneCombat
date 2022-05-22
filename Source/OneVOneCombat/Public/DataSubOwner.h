// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */

template<typename DataType>
class DataOwner;

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

	inline DataType* operator->() const
	{
		return data;
	}

	int32 GetId() { return id; }
	bool IsDataAvailable() { return data != nullptr; }

protected:
	DataType* data;

private:
	void* operator new(size_t);
	void* operator new(size_t, void*);
	void* operator new[](size_t);
	void* operator new[](size_t, void*);

	int32 id = -1;
	DataOwner<DataType>* dataOwner;
};

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