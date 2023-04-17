// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericEnums.generated.h"

UENUM(BlueprintType)
enum class E_CardinalDirection : uint8
{
	CD_Up UMETA(DisplayName = "North"),
	CD_Down UMETA(DisplayName = "South"),
	CD_Right UMETA(DisplayName = "East"),
	CD_Left UMETA(DisplayName = "West")
};