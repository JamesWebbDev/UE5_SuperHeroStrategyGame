// Fill out your copyright notice in the Description page of Project Settings.


#include "AComp_Grid.h"

// Sets default values for this component's properties
UAComp_Grid::UAComp_Grid()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAComp_Grid::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UAComp_Grid::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FVector2D UAComp_Grid::GetCurrentGridPosition() const
{
	return GridPosition;
}

