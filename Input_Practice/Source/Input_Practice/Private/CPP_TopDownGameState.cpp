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
		ACPP_TopDownControllerPlayer* PlayerController = Cast<ACPP_TopDownControllerPlayer>(ControllerPtr);
		ControllerArray.Add(PlayerController);
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



