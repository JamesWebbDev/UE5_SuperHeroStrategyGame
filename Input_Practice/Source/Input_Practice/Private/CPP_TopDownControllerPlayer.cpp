// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_TopDownControllerPlayer.h"

int32 ACPP_TopDownControllerPlayer::GetPlayerIndex() const
{
	return PlayerIndex;
}

void ACPP_TopDownControllerPlayer::SetPlayerIndex(int32 NewIndex)
{
	PlayerIndex = NewIndex;
}
