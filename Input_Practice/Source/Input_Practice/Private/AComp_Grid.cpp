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

	//DirectionValues.Add(FVector2D(0, 1), E_CardinalDirection::CD_Up);
	//DirectionValues.Add(FVector2D(1, 0), E_CardinalDirection::CD_Right);
	//DirectionValues.Add(FVector2D(0, -1), E_CardinalDirection::CD_Down);
	//DirectionValues.Add(FVector2D(-1, 0), E_CardinalDirection::CD_Left);

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
	const FVector MouseLookDirection = MousePosition - (Owner->GetActorLocation());
	const FVector2D LookDir2D = (FVector2D)MouseLookDirection.GetSafeNormal2D(0.0001, FVector::Zero());
	float ClosestDot = -2;
	FVector2D ClosestDir;

	TArray<FVector2D> DirKeys;
	DirectionValues.GetKeys(DirKeys);

	for (FVector2D Dir : DirKeys)
	{
		float Dot = FVector2D::DotProduct(LookDir2D, Dir);

		if (Dot > ClosestDot)
		{
			ClosestDot = Dot;
			ClosestDir = Dir;
		}
	}

	if (E_CardinalDirection* DirRef = DirectionValues.Find(ClosestDir))
	{
		E_CardinalDirection& Dir = *DirRef;
		OutTiles = RotatePositionsThenApplyOrigin(AffectedTiles_UpDir, GetCurrentLocationAtTile(), Dir);
	}
}

void UAComp_Grid::GetAttackableTiles_FixedDir(TArray<FVector2D> AffectedTiles_UpDir, E_CardinalDirection InDir, FVector2D InOrigin, TArray<FVector2D>& OutTiles)
{
	OutTiles = RotatePositionsThenApplyOrigin(AffectedTiles_UpDir, InOrigin, InDir);
}

void UAComp_Grid::GetSurroundingTiles(int32 Range, bool UseCurrentPosAsOrigin, TArray<FVector2D>& OutTiles)
{
	const FVector2D Origin = UseCurrentPosAsOrigin ? GetCurrentLocationAtTile() : GridPosition;
	const int32 NegativeRange = Range * -1;
	OutTiles.Add(Origin);

	for (int32 X = NegativeRange; X <= Range; X++)
	{
		for (int32 Y = NegativeRange; Y <= Range; Y++)
		{
			FVector2D TempGridPos = FVector2D(Origin.X + X, Origin.Y + Y);
			float Distance = FVector2D::Distance(TempGridPos, Origin);

			if (!Grid->IsTileValid(TempGridPos.X, TempGridPos.Y))
			{
				continue;
			}

			if (Distance > 0 && Distance <= Range + (Range / 10))
			{
				OutTiles.AddUnique(TempGridPos);
			}
		}
	}

}

TArray<FVector2D> UAComp_Grid::RotatePositionsThenApplyOrigin(TArray<FVector2D> TilePositions, FVector2D Origin, E_CardinalDirection RotateDirection)
{
	TArray<FVector2D> RotatedPositions;

	for (FVector2D Tile : TilePositions) 
	{
		float TempX = Tile.X;

		switch (RotateDirection)
		{
			case E_CardinalDirection::CD_Up: 
			{
				Tile.X = Tile.Y;
				Tile.Y = TempX * -1;
			} break;
			case E_CardinalDirection::CD_Down:
			{
				Tile.X = Tile.Y * -1;
				Tile.Y = TempX;
			} break;
			case E_CardinalDirection::CD_Right:
			{
				Tile.X = Tile.X;
				Tile.Y = Tile.Y;
			} break;
			case E_CardinalDirection::CD_Left:
			{
				Tile.X = Tile.X * -1;
				Tile.Y = Tile.Y * -1;
			} break;
		}

		Tile += Origin;

		RotatedPositions.Add(Tile);
	}

	return RotatedPositions;
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

