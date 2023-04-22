// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CPP_TopDownGameState.generated.h"

// ------- FORWARD DECLARATIONS ---------
class AAICharacter;
class CPP_TopDownControllerPlayer;


UCLASS()
class INPUT_PRACTICE_API ACPP_TopDownGameState : public AGameState
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP", Replicated)
	TArray<AAICharacter*> CharacterList;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(BlueprintPure, Category = "CPP")
		TArray<ACPP_TopDownControllerPlayer*> GetControllersTopDown() const;

	UFUNCTION(BlueprintCallable, Category = "CPP")
		void InitialiseCharacters();

	UFUNCTION(BlueprintCallable, Category = "CPP")
		void RemoveCharacterFromList(AAICharacter* Character);

	UFUNCTION(BlueprintCallable, Category = "CPP")
		void SetAllCharacterOwners();

	UFUNCTION(BlueprintCallable, Category = "CPP")
		TArray<AAICharacter*> GetCharactersOwnedByPlayers(int32 PlayerIndex);

	UFUNCTION(BlueprintCallable, Category = "CPP")
		bool GetCharacterAtGridPosition(FVector2D Position2D, AAICharacter*& FoundCharacter);

	UFUNCTION(BlueprintCallable, Category = "CPP")
		AAICharacter* GetNextActiveCharacter();

private:


};
