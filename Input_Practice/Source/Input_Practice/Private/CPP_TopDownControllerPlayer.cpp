// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_TopDownControllerPlayer.h"

int32 ACPP_TopDownControllerPlayer::GetPlayerIndexCPP() const
{
	return PlayerIndex;
}

void ACPP_TopDownControllerPlayer::SetPlayerIndexCPP(int32 NewIndex)
{
	PlayerIndex = NewIndex;
}
