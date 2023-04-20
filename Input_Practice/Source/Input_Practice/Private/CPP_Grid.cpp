// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Grid.h"
#include "AICharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACPP_Grid::ACPP_Grid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UE_LOG(LogTemp, Display, TEXT("------- START CONSTRUCTING -------"));

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("CPP_Scene"));
	LineProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("CPP_Line Procedural Mesh"));
	SelectionProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("CPP_Selection Procedural Mesh"));
	MoveProceduralMeshParent = CreateDefaultSubobject<USceneComponent>(TEXT("CPP_MoveParent"));
	AttackProceduralMeshParent = CreateDefaultSubobject<USceneComponent>(TEXT("CPP_AttackParent"));

	RootComponent = Scene;

	LineProceduralMesh->SetupAttachment(Scene);
	SelectionProceduralMesh->SetupAttachment(Scene);
	MoveProceduralMeshParent->SetupAttachment(Scene);
	AttackProceduralMeshParent->SetupAttachment(Scene);

	MeshMoveArray = GetMoveMeshChildren();
	MeshAttackArray = GetAttackMeshChildren();
}

// Called when the game starts or when spawned
void ACPP_Grid::BeginPlay()
{
	Super::BeginPlay();
}

UMaterialInstanceDynamic* ACPP_Grid::CreatMaterialInstance(const FLinearColor InColour, const float InOpacity)
{
	UMaterialInstanceDynamic* MatInst = UMaterialInstanceDynamic::Create(Material, NULL);

	MatInst->SetVectorParameterValue(FName("Colour"), InColour);
	MatInst->SetScalarParameterValue(FName("Opacity"), InOpacity);

	return MatInst;
}

void ACPP_Grid::CreateLine(FVector InStart, FVector InEnd, float InThickness, TArray<FVector>& InVertices, TArray<int32>& InTriangles)
{
	const float HalfThickness = InThickness / 2;
	const FVector yep = FVector((InEnd - InStart).Normalize(0.0001));
	const FVector yep2 = (InEnd - InStart).GetSafeNormal(0.0001, FVector::Zero());
	const FVector ThicknessDirection = FVector::CrossProduct(yep2, FVector(0, 0, 1));
	const FVector FacingThickness = ThicknessDirection * HalfThickness;
	const int32 InVerticeCount = InVertices.Num();
	
	TArray<FVector> TempVertices;
	TArray<int32> TempTriangles;

	// Add triangle points for single tile
	TempTriangles.Add(InVerticeCount + 2);
	TempTriangles.Add(InVerticeCount + 1);
	TempTriangles.Add(InVerticeCount + 0);
	TempTriangles.Add(InVerticeCount + 2);
	TempTriangles.Add(InVerticeCount + 3);
	TempTriangles.Add(InVerticeCount + 1);

	// Add Vertices for triangles to face toward for single tile
	TempVertices.Add(InStart	+ FacingThickness);
	TempVertices.Add(InEnd	+ FacingThickness);
	TempVertices.Add(InStart	- FacingThickness);
	TempVertices.Add(InEnd	- FacingThickness);

	InVertices.Append(TempVertices);
	InTriangles.Append(TempTriangles);
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

TArray<UProceduralMeshComponent*> ACPP_Grid::GetMoveMeshChildren() const
{
	TArray<USceneComponent*> TempMoveArray;
	TArray<UProceduralMeshComponent*> MoveMeshArray;

	MoveProceduralMeshParent->GetChildrenComponents(false, TempMoveArray);

	for (USceneComponent* MoveMesh : TempMoveArray)
	{
		MoveMeshArray.Add(Cast<UProceduralMeshComponent>(MoveMesh));
	}

	return MoveMeshArray;
}

TArray<UProceduralMeshComponent*> ACPP_Grid::GetAttackMeshChildren() const
{
	TArray<USceneComponent*> TempAttackArray;
	TArray<UProceduralMeshComponent*> MoveAttackArray;

	AttackProceduralMeshParent->GetChildrenComponents(false, TempAttackArray);

	for (USceneComponent* AttackMesh : TempAttackArray)
	{
		MoveAttackArray.Add(Cast<UProceduralMeshComponent>(AttackMesh));
	}

	return MoveAttackArray;
}

void ACPP_Grid::GenerateLineMeshData(TArray<FVector>& OutVertices, TArray<int32>& OutTriangles, UMaterialInstanceDynamic*& OutMaterial)
{
	OutMaterial = CreatMaterialInstance(LineColour, LineOpacity);

	float LineStart;
	const float LineHorizEnd = GetGridWidth();
	const float LineVertiEnd = GetGridHeight();

	UE_LOG(LogTemp, Display, TEXT("VerticeList Count before Lines: %d"), OutVertices.Num());

	for (int i = 0; i <= NumRows; i++)
	{
		LineStart = i * TileSize;

		CreateLine(FVector(LineStart, 0, 0), FVector(LineStart, LineHorizEnd, 0), LineThickness, OutVertices, OutTriangles);
	}

	UE_LOG(LogTemp, Display, TEXT("VerticeList Count MIDWAY Through Lines: %d"), OutVertices.Num());

	// Create Vertical Lines Geometry

	for (int i = 0; i <= NumColumns; i++)
	{
		LineStart = i * TileSize;

		CreateLine(FVector(0, LineStart, 0), FVector(LineVertiEnd, LineStart, 0), LineThickness, OutVertices, OutTriangles);
	}

	UE_LOG(LogTemp, Display, TEXT("VerticeList Count AFTER Lines: %d"), OutVertices.Num());
}

void ACPP_Grid::GenerateTileMeshData(E_TileMeshType Type, TArray<FVector>& OutVertices, TArray<int32>& OutTriangles, UMaterialInstanceDynamic*& OutMaterial)
{
	const float TileHalfSize = TileSize / 2;

	CreateLine(FVector(0, TileHalfSize, 0), FVector(TileSize, TileHalfSize, 0), TileSize, OutVertices, OutTriangles);
	
	switch (Type)
	{
		case E_TileMeshType::TMT_Select:
		{
			OutMaterial = OutMaterial = CreatMaterialInstance(SelectionColour, SelectionOpacity);
		};
		case E_TileMeshType::TMT_Move:
		{
			OutMaterial = OutMaterial = CreatMaterialInstance(MoveColour, MoveOpacity);
		};
		case E_TileMeshType::TMT_Attack:
		{
			OutMaterial = OutMaterial = CreatMaterialInstance(AttackColour, AttackOpacity);
		};
	}
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

void ACPP_Grid::SetHighlightedTilesState(E_PlayerActions Action, AAICharacter* InCharacter)
{
	switch (Action)
	{
		case E_PlayerActions::PA_Move:
		{
			SetMoveableTilesState(true);
			SetAttackableTilesState(false);

			if (InCharacter != nullptr)
			{
				InCharacter->ActivateMoveableTiles();
			}
		};
		case E_PlayerActions::PA_Attack:
		{
			SetMoveableTilesState(false);
			SetAttackableTilesState(true);

			if (InCharacter != nullptr)
			{
				InCharacter->ActivateAttackableTiles();
			}
		};
	}
}

void ACPP_Grid::SetTilePosition(UProceduralMeshComponent* Mesh, const FVector2D InTilePos, const FVector ActorLocation, const bool IsExtraTile)
{
	if (IsExtraTile)
	{
		Mesh->SetVisibility(false, false);
		return;
	}

	FVector2D TileWorldPos;
	bool IsValidLocation = TileToGridWorldLocation(FMath::TruncToInt32(InTilePos.X), FMath::TruncToInt32(InTilePos.Y), false, TileWorldPos);

	if (!IsValidLocation)
	{
		Mesh->SetVisibility(false, false);
		return;
	}

	Mesh->SetVisibility(true, false);
	Mesh->SetWorldLocation(FVector(TileWorldPos.X, TileWorldPos.Y, ActorLocation.Z), false, false);
}

void ACPP_Grid::SetSelectedTileState(bool NewValue)
{
	HighlightSelectedTile = NewValue;
}

void ACPP_Grid::SetSelectedTilePosition(int32 InRow, int32 InColumn)
{
	FVector2D OutLocation;
	bool IsValidInput = TileToGridWorldLocation(InRow, InColumn, false, OutLocation);

	if (!IsValidInput || !HighlightSelectedTile)
	{
		SelectionProceduralMesh->SetVisibility(false, false);
		return;
	}

	SelectionProceduralMesh->SetVisibility(true, false);

	float ZLocation = GetActorLocation().Z + 10;

	Scene->SetWorldLocation(FVector(OutLocation.X, OutLocation.Y, ZLocation), false, false);
}

void ACPP_Grid::SetMoveableTilesState(bool NewValue)
{
	HighlightMoveableTiles = NewValue;

	if (!HighlightMoveableTiles)
	{
		TArray<FVector2D> EmptyArray;
		SetMoveableTilesPositions(EmptyArray);
	}
}

void ACPP_Grid::SetMoveableTilesPositions(TArray<FVector2D> InGridLocations)
{
	if (!HighlightMoveableTiles)
	{
		for (int i = 0; i <= NumMoveMeshes; i++)
		{
			if (UProceduralMeshComponent* Mesh = MeshMoveArray[i])
			{
				Mesh->SetVisibility(false, false);
			}
		}
		return;
	}

	const int32 LocationCount = InGridLocations.Num();
	const FVector ActorLocation = GetActorLocation();

	if (LocationCount > NumMoveMeshes)
	{
		UE_LOG(LogTemp, Error, TEXT("Desired Moveable tile count EXCEEDS amount generated by %d!"), LocationCount - NumMoveMeshes);
		return;
	}

	for (int i = 0; i <= NumMoveMeshes; i++)
	{
		UProceduralMeshComponent* Mesh = MeshMoveArray[i];
		SetTilePosition(Mesh, InGridLocations[i], ActorLocation, i >= LocationCount);
	}

}

void ACPP_Grid::SetAttackableTilesState(bool NewValue)
{
	HighlightAttackableTiles = NewValue;

	if (!HighlightAttackableTiles)
	{
		TArray<FVector2D> EmptyArray;
		SetAttackableTilesPositions(EmptyArray);
	}
}

void ACPP_Grid::SetAttackableTilesPositions(TArray<FVector2D> InGridLocations)
{
	if (!HighlightAttackableTiles)
	{
		for (int i = 0; i <= NumAttackMeshes; i++)
		{
			if (UProceduralMeshComponent* Mesh = MeshAttackArray[i])
			{
				Mesh->SetVisibility(false, false);
			}
		}
		return;
	}

	const int32 LocationCount = InGridLocations.Num();
	const FVector ActorLocation = GetActorLocation();

	if (LocationCount > NumAttackMeshes)
	{
		UE_LOG(LogTemp, Error, TEXT("Desired Moveable tile count EXCEEDS amount generated by %d!"), LocationCount - NumMoveMeshes);
		return;
	}

	for (int i = 0; i <= NumAttackMeshes; i++)
	{
		UProceduralMeshComponent* Mesh = MeshAttackArray[i];
		SetTilePosition(Mesh, InGridLocations[i], ActorLocation, i >= LocationCount);
	}
}

