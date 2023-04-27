// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AI_Grid.h"

// Sets default values
ACPP_AI_Grid::ACPP_AI_Grid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UserComponent = CreateDefaultSubobject<UAComp_GridUser>(TEXT("GridUserComponent"));
}

// Called when the game starts or when spawned
void ACPP_AI_Grid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_AI_Grid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

