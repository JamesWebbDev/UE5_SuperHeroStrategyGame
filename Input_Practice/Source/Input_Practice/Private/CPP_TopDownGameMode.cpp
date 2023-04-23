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
	DOREPLIFETIME(ACPP_TopDownGameMode, SelectedAICharacter);
}

AAICharacter* ACPP_TopDownGameMode::GetSelectedCharacter() const
{
	return SelectedAICharacter;
}
