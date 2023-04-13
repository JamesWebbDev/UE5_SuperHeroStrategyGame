// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GenericStructs.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTestStruct
{
	GENERATED_USTRUCT_BODY()

	FORCEINLINE FTestStruct();

	explicit FORCEINLINE FTestStruct(int32 InValue);

	explicit FORCEINLINE FTestStruct(int32 InX, int32 InY);

	UPROPERTY(BlueprintReadWrite)
		int32 X = 0;

	UPROPERTY(BlueprintReadWrite)
		int32 Y = 0;

	bool operator==(const FTestStruct& V) const;
	bool operator!=(const FTestStruct& V) const;
};

FORCEINLINE FTestStruct::FTestStruct() 
{

}

FORCEINLINE FTestStruct::FTestStruct(const int32 InValue) : X(InValue), Y(InValue)
{

}

FORCEINLINE FTestStruct::FTestStruct(const int32 InX, const int32 InY) : X(InX), Y(InY)
{

}

FORCEINLINE bool FTestStruct::operator==(const FTestStruct& V) const
{
	return X == V.X && Y == V.Y;
}

FORCEINLINE bool FTestStruct::operator!=(const FTestStruct& V) const 
{
	return X != V.X || Y != V.Y;
}

