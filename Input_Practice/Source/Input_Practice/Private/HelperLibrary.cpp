// Fill out your copyright notice in the Description page of Project Settings.


#include "HelperLibrary.h"

UHelperLibrary::UHelperLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

FVector2D UHelperLibrary::GetClosestV2_ToTarget(const FVector2D InTarget, TArray<FVector2D> InList, const bool UseSquaredDistance = false, const float InDefaultDistance)
{
	FVector2D ClosestV2 = FVector2D(-1000);
	float TempDistance;
	float ClosestDistance = InDefaultDistance;

	for (FVector2D Pos : InList)
	{
		TempDistance = UseSquaredDistance ? 
			FVector2D::DistSquared(InTarget, Pos) : 
			FVector2D::Distance(InTarget, Pos);

		if (TempDistance < ClosestDistance)
		{
			ClosestDistance = TempDistance;
			ClosestV2 = Pos;
		}
	}

	return ClosestV2;
}

FVector2D UHelperLibrary::GetFurthestV2_FromTarget(const FVector2D InTarget, TArray<FVector2D> InList, const bool UseSquaredDistance = false, const float InDefaultDistance)
{
	FVector2D FurthestV2 = FVector2D(-1000);
	float TempDistance;
	float FurthestDistance = InDefaultDistance;

	for (FVector2D Pos : InList)
	{
		TempDistance = UseSquaredDistance ?
			FVector2D::DistSquared(InTarget, Pos) :
			FVector2D::Distance(InTarget, Pos);

		if (TempDistance > FurthestDistance)
		{
			FurthestDistance = TempDistance;
			FurthestV2 = Pos;
		}
	}

	return FurthestV2;
}
