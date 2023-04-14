// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPP_TopDownControllerPlayer.generated.h"

/**
 * 
 */
UCLASS()
class INPUT_PRACTICE_API ACPP_TopDownControllerPlayer : public APlayerController
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, Category = "CPP")
		int32 PlayerIndex;

public: 

	UFUNCTION(BlueprintPure, Category = "CPP")
		int32 GetPlayerIndex() const;
};
