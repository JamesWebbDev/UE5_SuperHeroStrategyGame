// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridUser.h"
#include "CPP_AI_Grid.generated.h"

UCLASS()
class INPUT_PRACTICE_API ACPP_AI_Grid : public AActor, public IGridUser
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_AI_Grid();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|Components")
		UAComp_GridUser* UserComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
