// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GenericEnums.h"
#include "CPP_TopDownGameMode.generated.h"

class AAICharacter;


/**
 * 
 */
UCLASS()
class INPUT_PRACTICE_API ACPP_TopDownGameMode : public AGameMode
{
	GENERATED_BODY()
	
protected: 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CPP|Replicated", Replicated)
		AAICharacter* SelectedAICharacter;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	
public: 

	// ----------- Variables --------------

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		AAICharacter* GetSelectedCharacter() const;

	// ----------- Events --------------

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "CPP|Events")
		void EndUserTurn(const E_PlayerActions EndingUserState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "CPP|Events")
		void CharacterFinalActionHasConcluded();

};
