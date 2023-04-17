// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_Attack.generated.h"

/**
 * 
 */
UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
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

	UPROPERTY(EditDefaultsOnly, Category = "CPP|Gameplay", meta = (ToolTip = "Defines the distance of a targeted attack."))
		int32 AttackRange;

	UPROPERTY(EditDefaultsOnly, Category = "CPP|Gameplay", meta = (ToolTip = "Can this attack hit multiple tiles at once?"))
		bool IsAOE;

	UPROPERTY(EditDefaultsOnly, Category = "CPP|Gameplay", meta = (ToolTip = "Defines tiles affected by AOE attacks. (Facing North)", ClampMin=0, ClampMax=4))
		TArray<FVector2D> AttackTiles;

	UPROPERTY(EditDefaultsOnly, Category = "CPP|Gameplay")
		TSubclassOf<class UAnimInstance> AnimationBP;


public: 

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		FText GetName() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		FText GetDescription() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		UTexture2D* GetImage() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		int32 GetDamage() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		int32 GetAttackRange() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		bool GetisAOE() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		TArray<FVector2D> GetAttackTiles() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		TSubclassOf<class UAnimInstance> GetAnimInstance() const;
};
