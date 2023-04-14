// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_TopDownGameState.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"

/// <summary>
/// Replicate Variables in Class.
/// </summary>
/// <param name="OutLifetimeProps"></param>
void ACPP_TopDownGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACPP_TopDownGameState, CharacterList);
}

/// <summary>
/// Get Existing 'Top Down Controllers' (Players)
/// </summary>
/// <returns></returns>
TArray<ACPP_TopDownControllerPlayer*>  ACPP_TopDownGameState::GetControllersTopDown() const
{
	TArray<ACPP_TopDownControllerPlayer*> ControllerArray;

	for (TObjectPtr<APlayerState> PlayerState : PlayerArray)
	{
		APlayerController* ControllerPtr = PlayerState.Get()->GetPlayerController();
		ControllerArray.Add(Cast<ACPP_TopDownControllerPlayer>(ControllerPtr));
	}

	UE_LOG(LogTemp, Warning, TEXT("Successfully got all TopDown controllers without error!"));

	return ControllerArray;
}

void ACPP_TopDownGameState::InitialiseCharacters()
{
	TArray<AActor*> CharacterArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAICharacter::StaticClass(), CharacterArray);

	for (AActor* Actor : CharacterArray) 
	{
		CharacterList.Add(Cast<AAICharacter>(Actor));
	}

	
}

void ACPP_TopDownGameState::RemoveCharacterFromList(AAICharacter* Character)
{
	if (CharacterList.Contains(Character)) 
	{
		CharacterList.Remove(Character);
	}
}

void ACPP_TopDownGameState::SetAllCharacterOwners()
{
	TArray<ACPP_TopDownControllerPlayer*> Players = GetControllersTopDown();

	for (AAICharacter* Character : CharacterList)
	{
		for (ACPP_TopDownControllerPlayer* Player : Players)
		{
			int32 PIndex = Player->GetPlayerIndex();
			int32 CIndex = Character->GetPlayerIndex();

			if (PIndex == CIndex) 
			{
				Character->SetOwner(Player);
			}
		}
	}

	
}

TArray<AAICharacter*> ACPP_TopDownGameState::GetAPlayersCharacters(int32 PlayerIndex)
{
	TArray<AAICharacter*> MatchingCharacters;

	for (AAICharacter* Character : CharacterList)
	{
		int32 CharIndex = Character->GetPlayerIndex();

		if (PlayerIndex == CharIndex)
		{
			MatchingCharacters.Add(Character);
		}
	}

	return MatchingCharacters;
}

bool ACPP_TopDownGameState::GetCharacterAtGridPosition(FVector2D Position2D, AAICharacter*& FoundCharacter)
{
	for (AAICharacter* Character : CharacterList)
	{
		FVector2D CharPosition = Character->GridComponent->GetCurrentGridPosition();

		if (Position2D.Equals(CharPosition, 0))
		{
			FoundCharacter = Character;
			return true;
		}
	}

	return false;
}

AAICharacter* ACPP_TopDownGameState::GetNextActiveCharacter()
{
	AAICharacter* NextCharacter = nullptr;

	for (AAICharacter* Character : CharacterList)
	{
		if (Character->GetHasActedThisRotation())
		{
			continue;
		}

		if (NextCharacter == nullptr)
		{
			NextCharacter = Character;
			continue;
		}

		if (Character->GetSpeed() > NextCharacter->GetSpeed())
		{
			NextCharacter = Character;
		}

	}

	if (NextCharacter != nullptr)
	{
		return NextCharacter;
	}


	// Update This part of the code to reset HasActed bools and restart function
	return nullptr;
}




