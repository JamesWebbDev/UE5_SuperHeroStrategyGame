// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Grid.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACPP_Grid::ACPP_Grid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("CPP_Scene"));
	LineProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("CPP_Line Procedural Mesh"));
	SelectionProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("CPP_Selection Procedural Mesh"));
	MoveProceduralMeshParent = CreateDefaultSubobject<USceneComponent>(TEXT("CPP_MoveParent"));
	AttackProceduralMeshParent = CreateDefaultSubobject<USceneComponent>(TEXT("CPP_AttackParent"));

	Scene->SetupAttachment(GetRootComponent());
	LineProceduralMesh->SetupAttachment(Scene);
	SelectionProceduralMesh->SetupAttachment(Scene);
	MoveProceduralMeshParent->SetupAttachment(Scene);
	AttackProceduralMeshParent->SetupAttachment(Scene);


	MoveProceduralMeshParent->GetChildrenComponents(false, MeshMoveArray);
	AttackProceduralMeshParent->GetChildrenComponents(false, AttackMoveArray);
}

// Called when the game starts or when spawned
void ACPP_Grid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_Grid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ACPP_Grid::IsTileValid(int32 Row, int32 Column) const
{
	if (Row >= 0 &&
		Row < NumRows &&
		Column >= 0 &&
		Column < NumColumns)
	{
		return true;
	}

	return false;
}

float ACPP_Grid::GetTileSize() const
{
	return TileSize;
}

float ACPP_Grid::GetGridWidth() const
{
	return NumRows * TileSize;
}

float ACPP_Grid::GetGridHeight() const
{
	return NumColumns * TileSize;
}

bool ACPP_Grid::LocationToTile(const FVector Location, int32& OutRow, int32& OutColumn)
{
	const FVector ActorLocation = GetActorLocation();

	OutRow = FMath::FloorToInt(((Location.X - ActorLocation.X) / GetGridWidth()) * NumRows);
	OutColumn = FMath::FloorToInt(((Location.Y - ActorLocation.Y) / GetGridHeight()) * NumColumns);

	return IsTileValid(OutRow, OutColumn);
}

bool ACPP_Grid::TileToGridWorldLocation(const int32 InRow, const int32 InColumn, const bool GetCenter, FVector2D& OutGridLocation)
{
	FVector2D GridLocation;
	const FVector ActorLocation = GetActorLocation();
	const float CenterFactor = GetCenter ? TileSize / 2 : 0;

	GridLocation.X = (InRow * TileSize) + ActorLocation.X + CenterFactor;
	GridLocation.Y = (InColumn * TileSize) + ActorLocation.Y + CenterFactor;

	OutGridLocation = GridLocation;

	return IsTileValid(InRow, InColumn);
}

