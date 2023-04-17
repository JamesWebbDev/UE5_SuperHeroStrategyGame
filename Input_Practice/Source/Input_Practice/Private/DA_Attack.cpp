// Fill out your copyright notice in the Description page of Project Settings.


#include "DA_Attack.h"

FText UDA_Attack::GetName() const
{
	return Name;
}

FText UDA_Attack::GetDescription() const
{
	return Description;
}

UTexture2D* UDA_Attack::GetImage() const
{
	return Image;
}

int32 UDA_Attack::GetDamage() const
{
	return Damage;
}

int32 UDA_Attack::GetAttackRange() const
{
	return AttackRange;
}

bool UDA_Attack::GetisAOE() const
{
	return IsAOE;
}

TArray<FVector2D> UDA_Attack::GetAttackTiles() const
{
	return AttackTiles;
}

TSubclassOf<class UAnimInstance> UDA_Attack::GetAnimInstance() const
{
	return AnimationBP;
}
