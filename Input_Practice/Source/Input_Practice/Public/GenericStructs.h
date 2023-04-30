// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DA_Attack.h"
#include "GenericEnums.h"
#include "GenericStructs.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FScenario
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CPP")
		int32 Score = -10000;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CPP")
		FVector2D MovePosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CPP")
		bool bIsAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CPP")
		UDA_Attack* AttackType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CPP")
		TArray<FVector2D> AttackPositions;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CPP")
		E_CardinalDirection AttackDirection = E_CardinalDirection::CD_Up;
};
