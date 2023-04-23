// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_TopDownGameState.h"
#include "AICharacter.h"
#include "CPP_TopDownControllerPlayer.h"
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
	DOREPLIFETIME(ACPP_TopDownGameState, GridUserList);
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

AActor* ACPP_TopDownGameState::GetUserByIndex(int32 InIndex) const
{
	if (InIndex >= GridUserList.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("GameState does NOT have User in List at Index %d!!!"), InIndex);
		return nullptr;
	}

	return GridUserList[InIndex];
}

void ACPP_TopDownGameState::SortGridUsersByIndex()
{
	const int Length = GridUserList.Num();
	const TArray<AActor*> ListCopy = GridUserList;
	TArray<int32> IndexArray;

	for (int x = 0; x < Length; x++)
	{
		IndexArray.Add(IGridUser::Execute_GetGridUser(GridUserList[x])->GetUserIndex());
	}


	int i, j, temp, pass = 0;

	for (i = 0; i < Length; i++) {
		for (j = i + 1; j < Length; j++)
		{
			if (IndexArray[j] < IndexArray[i])
			{
				temp = IndexArray[i];
				IndexArray[i] = IndexArray[j];
				IndexArray[j] = temp;
			}
		}
		pass++;
	}

	for (int y = 0; y < Length; y++)
	{
		GridUserList[y] = ListCopy[IndexArray[y]];
	}
}

void ACPP_TopDownGameState::SetGridUsersList()
{
	TArray<AActor*> UserArray;

	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UGridUser::StaticClass(), UserArray);

	GridUserList = UserArray;
	SortGridUsersByIndex();
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
	SetGridUsersList();

	for (AAICharacter* Character : CharacterList)
	{
		for (AActor* User : GridUserList)
		{
			int32 UIndex = IGridUser::Execute_GetGridUser(User)->GetUserIndex();
			int32 CIndex = Character->GetPlayerIndex();

			if (UIndex == CIndex && Cast<ACPP_TopDownControllerPlayer>(User)) 
			{
				Character->SetOwner(User);
				Character->SetOwningUser(User);
			}
		}
	}
}

TArray<AAICharacter*> ACPP_TopDownGameState::GetCharactersOwnedByPlayers(int32 PlayerIndex)
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
	FoundCharacter = nullptr;

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

		if (Character->GetMoveRange() > NextCharacter->GetMoveRange())
		{
			NextCharacter = Character;
		}

	}

	if (NextCharacter != nullptr)
	{
		return NextCharacter;
	}

	for (AAICharacter* Character : CharacterList)
	{
		Character->Event_MultiRPC_ActionTaken(false);
	}

	// Update This part of the code to reset HasActed bools and restart function
	return GetNextActiveCharacter();
}




