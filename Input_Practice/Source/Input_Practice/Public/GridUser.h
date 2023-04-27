// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AComp_GridUser.h"
#include "GridUser.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UGridUser : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INPUT_PRACTICE_API IGridUser
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CPP|Grid User") 
		UAComp_GridUser* GetGridUser() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CPP|GridUser")
		void OnTurnStart();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CPP|GridUser")
		void OnTurnEnd();
};
