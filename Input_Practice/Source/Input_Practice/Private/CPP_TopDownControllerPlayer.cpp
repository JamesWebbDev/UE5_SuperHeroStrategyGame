// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_TopDownControllerPlayer.h"

ACPP_TopDownControllerPlayer::ACPP_TopDownControllerPlayer()
{
	UserComponent = CreateDefaultSubobject<UAComp_GridUser>(TEXT("GridUserComponent"));
}

int32 ACPP_TopDownControllerPlayer::GetPlayerIndexCPP() const
{
	return PlayerIndex;
}

void ACPP_TopDownControllerPlayer::SetPlayerIndexCPP(int32 NewIndex)
{
	PlayerIndex = NewIndex;
}
