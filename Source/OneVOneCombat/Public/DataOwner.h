// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DataReadOwner.h"

#include "EditorUtilities.h"

/**
 * 
 */

template<typename DataType>
class ONEVONECOMBAT_API DataOwner
{
public:
	DataOwner() = default;
	DataOwner(DataType* data) : data(data) {}

	void BeSubOwner(DataSubOwner<DataType>* dataSubOwner)
	{
		checkf(dataSubOwner, TEXT("DataSubOwner is null"));

		if(dataSubOwner->IsDataAvailable())
		{
			LOG_TO_SCREEN("ERROR: Data is not null, it may be already a data sub owner! BeSubOwner is returned!");
			return;
		}

		int32 id = -1;

		for (int32 i = 0; i < subOwners.Num(); ++i)
		{
			if (subOwners[i] == nullptr)
			{
				id = i;
				subOwners[i] = dataSubOwner;
			}
		}

		if (id == -1)
		{
			id = subOwners.Num();
			subOwners.Add(dataSubOwner);
		}

		dataSubOwner->ResetSubOwner(id, data, this);
	}

	void BeReadOwner(DataReadOwner<DataType>* dataReadOwner)
	{
		BeSubOwner(dataReadOwner);
	}

	void OnSubOwnerDestroyed(DataSubOwner<DataType>* dataSubOwner)
	{
		checkf(dataSubOwner, TEXT("DataSubOwner is null"));

		const int32 id = dataSubOwner->GetId();
		subOwners[id] = nullptr;
		dataSubOwner->OnDataOwnerEndSubOwnership();
	}

	inline const DataType& GetData() { return *data; }

	~DataOwner()
	{
		for (int32 i = 0; i < subOwners.Num(); ++i)
		{
			if (subOwners[i])
			{
				subOwners[i]->OnDataOwnerEndSubOwnership();
			}
		}
	}

private:

	DataType* data;
	TArray<DataSubOwner<DataType>*> subOwners;

	// Disallow creating on heap. DataOwners shall be created on stack with its data.
	void* operator new(size_t);
	void* operator new(size_t, void*);
	void* operator new[](size_t);
	void* operator new[](size_t, void*);
};
