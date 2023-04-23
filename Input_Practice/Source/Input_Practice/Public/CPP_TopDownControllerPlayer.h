// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AComp_GridUser.h"
#include "GridUser.h"
#include "CPP_TopDownControllerPlayer.generated.h"

/**
 * 
 */
UCLASS()
class INPUT_PRACTICE_API ACPP_TopDownControllerPlayer : public APlayerController, public IGridUser
{
	GENERATED_BODY()
	
public: 
	// Sets default values for this character's properties
	ACPP_TopDownControllerPlayer();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|Components")
		UAComp_GridUser* UserComponent;

private:

	UPROPERTY(VisibleAnywhere, Category = "CPP")
		int32 PlayerIndex;

protected: 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CPP")
		float RotateSensitivity;

public: 

	UFUNCTION(BlueprintPure, Category = "CPP")
		int32 GetPlayerIndexCPP() const;

	UFUNCTION(BlueprintCallable, Category = "CPP")
		void SetPlayerIndexCPP(int32 NewIndex);
};
