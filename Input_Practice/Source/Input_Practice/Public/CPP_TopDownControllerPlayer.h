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
	
public: 
	// Sets default values for this character's properties
	ACPP_TopDownControllerPlayer();

protected: 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CPP")
		float RotateSensitivity;
};
