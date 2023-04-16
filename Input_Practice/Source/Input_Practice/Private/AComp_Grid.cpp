// Fill out your copyright notice in the Description page of Project Settings.

#include "AComp_Grid.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


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

	Owner = GetOwner();
	Grid = Cast<ACPP_Grid>(UGameplayStatics::GetActorOfClass(GetWorld(), ACPP_Grid::StaticClass()));
	
	if (Grid == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't find Grid reference in World!"));
		return;
	}

	SetGridPosition(GetCurrentLocationAtTile());
	SetWorldPositionFromGridPosition();
}


// Called every frame
void UAComp_Grid::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

ACPP_Grid* UAComp_Grid::GetGrid() const
{
	return Grid;
}

FVector2D UAComp_Grid::GetCurrentGridPosition() const
{
	return GridPosition;
}

void UAComp_Grid::SetGridPosition(FVector2D NewPosition)
{
	GridPosition = NewPosition;
}

void UAComp_Grid::SetWorldPositionFromGridPosition()
{
	if (Grid == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't get Grid reference on Grid Component."));
		return;
	}

	const FVector ActorLocation = Owner->GetActorLocation();
	FVector2D OutWorldLocation;

	if (Grid->TileToGridWorldLocation(GridPosition.X, GridPosition.Y, true, OutWorldLocation))
	{
		FVector NewPosition = FVector(OutWorldLocation.X, OutWorldLocation.Y, ActorLocation.Z);

		Owner->SetActorLocation(NewPosition);
	}
}

void UAComp_Grid::SetWorldPositionFromCurrentPosition()
{
	if (Grid == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't get Grid reference on Grid Component."));
		return;
	}

	const FVector ActorLocation = Owner->GetActorLocation();
	const FVector2D TileWorldLocation = GetCurrentLocationAtTile();
	FVector2D OutWorldLocation;

	if (Grid->TileToGridWorldLocation(TileWorldLocation.X, TileWorldLocation.Y, true, OutWorldLocation))
	{
		FVector NewPosition = FVector(OutWorldLocation.X, OutWorldLocation.Y, ActorLocation.Z);

		Owner->SetActorLocation(NewPosition);
	}


}

void UAComp_Grid::GetAttackableTiles(TArray<FVector2D> AffectedTiles_UpDir, FVector MousePosition, TArray<FVector2D>& OutTiles)
{

}

FVector2D UAComp_Grid::GetCurrentLocationAtTile()
{
	if (Grid == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't get Grid reference on Grid Component."));
		return FVector2D::Zero();
	}

	const FVector ActorLocation = Owner->GetActorLocation();
	int32 Row;
	int32 Column;

	if (Grid->LocationToTile(ActorLocation, Row, Column))
	{
		return FVector2D(Row, Column);
	}

	return FVector2D::Zero();
}

