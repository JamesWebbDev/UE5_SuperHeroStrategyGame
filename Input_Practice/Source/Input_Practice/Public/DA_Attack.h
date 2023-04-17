// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_Attack.generated.h"

/**
 * 
 */
UCLASS()
class INPUT_PRACTICE_API UDA_Attack : public UDataAsset
{
	GENERATED_BODY()
	
private: 

	UPROPERTY(EditDefaultsOnly, Category = "CPP|Info")
		FText Name;

	UPROPERTY(EditDefaultsOnly, Category = "CPP|Info")
		FText Description;

	UPROPERTY(EditDefaultsOnly, Category = "CPP|Info")
		UTexture2D* Image;

	UPROPERTY(EditDefaultsOnly, Category = "CPP|Gameplay")
		int32 Damage;

	UPROPERTY(EditDefaultsOnly, Category = "CPP|Gameplay")
		int32 TargetRange;

	UPROPERTY(EditDefaultsOnly, Category = "CPP|Gameplay")
		bool IsAOE;

	UPROPERTY(EditDefaultsOnly, Category = "CPP|Gameplay")
		TArray<FVector2D> AttackTiles;

	UPROPERTY(EditDefaultsOnly, Category = "CPP|Gameplay")
		TSubclassOf<class UAnimInstance> AnimationBP;

};
