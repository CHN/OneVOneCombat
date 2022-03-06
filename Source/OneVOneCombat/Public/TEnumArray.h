// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

template<typename ValueType, typename EnumType>
class ONEVONECOMBAT_API TEnumArray
{
	using NonPtrValueType = std::remove_pointer_t<ValueType>;
	using ArrayType = TArray<ValueType>;
	using SizeType = typename ArrayType::SizeType;

	static constexpr bool IsPointerType = std::is_pointer_v<ValueType>;
public:

	TEnumArray()
	{
		ReserveArray();
	}

	TEnumArray(std::initializer_list<ValueType> list)
	{
		checkf(list.size() <= static_cast<SizeType>(EnumType::END_OF_ENUM), TEXT("List size is greater than array size"));

		ReserveArray();
		array = list;
	}

	TEnumArray(const TEnumArray& inArray)
	{
		array = inArray;
	}

	TEnumArray(TEnumArray&& inArray)
	{
		std::move(array, inArray);
	}

	void operator=(const TEnumArray& inArray)
	{
		array = inArray;
	}

	void operator=(TEnumArray&& inArray)
	{
		std::move(array, inArray);
	}

	template<typename T = bool, typename std::enable_if_t<IsPointerType, T> = true>
	NonPtrValueType& operator[](EnumType enumIndex)
	{
		return *array[static_cast<SizeType>(enumIndex)];
	}

	template<typename T = bool, typename std::enable_if_t<IsPointerType, T> = true>
	const NonPtrValueType& operator[](EnumType enumIndex) const
	{
		return *array[static_cast<SizeType>(enumIndex)];
	}

	template<typename T = bool, typename std::enable_if_t<!IsPointerType, T> = true>
	ValueType& operator[](EnumType enumIndex)
	{
		return array[static_cast<SizeType>(enumIndex)];
	}

	template<typename T = bool, typename std::enable_if_t<!IsPointerType, T> = true>
	const ValueType& operator[](EnumType enumIndex) const
	{
		return array[static_cast<SizeType>(enumIndex)];
	}

	ArrayType& GetUnderlyingArray()
	{
		return array;
	}

	constexpr SizeType Num()
	{
		return static_cast<SizeType>(EnumType::END_OF_ENUM);
	}

private:

	void ReserveArray()
	{
		checkf(static_cast<SizeType>(EnumType::END_OF_ENUM) > 0, TEXT("EnumType::END_OF_ENUM not available or lower or equal to 0"));

		array.SetNum(static_cast<SizeType>(EnumType::END_OF_ENUM));
	}

	ArrayType array;
};
