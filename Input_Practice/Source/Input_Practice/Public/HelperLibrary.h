// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HelperLibrary.generated.h"

/**
 * 
 */
UCLASS()
class INPUT_PRACTICE_API UHelperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

		UFUNCTION(BlueprintCallable, Category = "Helpers|Vector2D", meta = (KeyWords = "Distance Closest Vector2D")) 
		static FVector2D GetClosestV2_ToTarget(const FVector2D InTarget, TArray<FVector2D> InList, const bool UseSquaredDistance, const float InDefaultDistance = 100000);
	
		UFUNCTION(BlueprintCallable, Category = "Helpers|Vector2D", meta = (KeyWords = "Distance Furthest Vector2D"))
		static FVector2D GetFurthestV2_FromTarget(const FVector2D InTarget, TArray<FVector2D> InList, const bool UseSquaredDistance, const float InDefaultDistance = -100000);
};
