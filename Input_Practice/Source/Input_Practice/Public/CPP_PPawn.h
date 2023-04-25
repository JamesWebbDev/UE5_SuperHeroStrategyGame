// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GridUser.h"
#include "CPP_PPawn.generated.h"

UCLASS()
class INPUT_PRACTICE_API ACPP_PPawn : public APawn, public IGridUser
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACPP_PPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|Components")
		UAComp_GridUser* UserComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
