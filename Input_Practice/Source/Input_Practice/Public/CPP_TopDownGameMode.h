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

	// ----------- Variables --------------

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "CPP|Replicated", Replicated)
		AAICharacter* SelectedAICharacter;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CPP|Networking")
		bool bIsNetworkedLevel = false;

public: 


	// ----------- Functions --------------

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		AAICharacter* GetSelectedCharacter() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		bool GetIsNetworked() const;

	UFUNCTION(BlueprintCallable, Category = "CPP|Constants")
		void SetIsNetworked(bool Value);

	// ----------- Events --------------

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "CPP|Events")
		void StartGame();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "CPP|Events")
		void StartGameWithAI();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "CPP|Events")
		void EndUserTurn(const E_PlayerActions EndingUserState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "CPP|Events")
		void CharacterFinalActionHasConcluded();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "CPP|Events")
		void DestroyCharacter(AAICharacter* InCharacter, int32 OwningPlayerIndex);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "CPP|Events")
		void EndGame(int32 WinningPlayerIndex);

};
