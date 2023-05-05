// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_TopDownGameMode.h"
#include "Net/UnrealNetwork.h"

/// <summary>
/// Replicate Variables in Class.
/// </summary>
/// <param name="OutLifetimeProps"></param>
void ACPP_TopDownGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	if (!bIsNetworkedLevel)
		return; 

	DOREPLIFETIME(ACPP_TopDownGameMode, SelectedAICharacter);
}

AAICharacter* ACPP_TopDownGameMode::GetSelectedCharacter() const
{
	return SelectedAICharacter;
}

bool ACPP_TopDownGameMode::GetIsNetworked() const
{
	return bIsNetworkedLevel;
}

void ACPP_TopDownGameMode::SetIsNetworked(bool Value)
{
	bIsNetworkedLevel = Value;
}
