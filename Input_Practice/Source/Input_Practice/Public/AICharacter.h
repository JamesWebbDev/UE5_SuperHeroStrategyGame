// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AComp_Grid.h"
#include "AComp_Health.h"
#include "AComp_Attack.h"
#include "AICharacter.generated.h"

UCLASS()
class INPUT_PRACTICE_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP")
		UAComp_Grid* GridComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP")
		UAComp_Health* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP")
		UAComp_Attack* AttackComponent;

protected: 

	UPROPERTY(VisibleAnywhere, Category = "CPP")
		int32 PlayerIndex;

	UPROPERTY(VisibleAnywhere, Category = "CPP")
		bool  HasActedThisRotation;

	UPROPERTY(VisibleAnywhere, EditAnywhere, Category = "CPP|Attributes")
		int32 Speed;

private: 

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		int32 GetPlayerIndex() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		int32 GetSpeed() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		bool GetHasActedThisRotation() const;

};
