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


	TArray<USceneComponent*> TempMoveArray;
	TArray<USceneComponent*> TempAttackArray;

	MoveProceduralMeshParent->GetChildrenComponents(false, TempMoveArray);
	AttackProceduralMeshParent->GetChildrenComponents(false, TempAttackArray);

	for (USceneComponent* MoveMesh : TempMoveArray)
	{
		MeshMoveArray.Add(Cast<UProceduralMeshComponent>(MoveMesh));
	}

	for (USceneComponent* AttackMesh : TempAttackArray)
	{
		MeshAttackArray.Add(Cast<UProceduralMeshComponent>(AttackMesh));
	}

	UMaterialInstance* LineMatInst = CreatMaterialInstance(LineColour, LineOpacity);
	UMaterialInstance* SelectedMatInst = CreatMaterialInstance(SelectionColour, SelectionOpacity);
	UMaterialInstance* MoveMatInst = CreatMaterialInstance(MoveColour, MoveOpacity);
	UMaterialInstance* AttackMatInst = CreatMaterialInstance(AttackColour, AttackOpacity);

	// Create Horizontal Lines Geometry
	TArray<FVector>* NormalsEmpty;
	TArray<FVector2D>* UVEmpty;
	TArray<FColor>* VertColoursEmpty;
	TArray<FProcMeshTangent>* TangentsEmpty;

	TArray<FVector>* LineVertices;
	TArray<int32>* LineTriangles;
	float LineStart;
	const float LineHorizEnd = GetGridWidth();
	const float LineVertiEnd = GetGridHeight();

	for (int i = 0; i <= NumRows; i++)
	{
		LineStart = i * TileSize;

		CreateLine(FVector(LineStart), FVector(LineStart, LineHorizEnd, 0), LineThickness, LineVertices, LineTriangles);
	}

	for (int i = 0; i <= NumColumns; i++)
	{
		LineStart = i * TileSize;

		CreateLine(FVector(0, LineStart, 0), FVector(LineVertiEnd, LineStart, 0), LineThickness, LineVertices, LineTriangles);
	}

	TArray<FVector> LineVerticesData = *LineVertices;
	TArray<int32> LineTriangleData = *LineTriangles;

	//UProceduralMeshComponent::CreateMeshSection_LinearColor(0, LineVerticesData.get, LineTriangleData);
	//UProceduralMeshComponent::CreateMeshSection_LinearColor();
	
}

// Called when the game starts or when spawned
void ACPP_Grid::BeginPlay()
{
	Super::BeginPlay();
	
}

UMaterialInstanceDynamic* ACPP_Grid::CreatMaterialInstance(const FLinearColor InColour, const float InOpacity)
{
	UMaterialInstanceDynamic* MatInst;
	UMaterialInstanceDynamic::Create(MatInst, this);

	MatInst->SetVectorParameterValue(FName("Colour"), InColour);
	MatInst->SetScalarParameterValue(FName("Opacity"), InOpacity);

	return MatInst;
}

void ACPP_Grid::CreateLine(FVector InStart, FVector InEnd, float InThickness, TArray<FVector>* InVertices, TArray<int32>* InTriangles)
{
	const float HalfThickness = InThickness / 2;
	const FVector ThicknessDirection = FVector::CrossProduct((InEnd - InStart).Normalize(0.0001), FVector(0, 0, 1));
	const FVector FacingThickness = ThicknessDirection * HalfThickness;
	const int32 InVerticeCount = InVertices->Num();
	

	// Add triangle points for single tile
	InTriangles->Add(InVerticeCount + 2);
	InTriangles->Add(InVerticeCount + 1);
	InTriangles->Add(InVerticeCount + 0);
	InTriangles->Add(InVerticeCount + 2);
	InTriangles->Add(InVerticeCount + 3);
	InTriangles->Add(InVerticeCount + 1);

	// Add Vertices for triangles to face toward for single tile
	InVertices->Add(InStart + FacingThickness);
	InVertices->Add(InEnd	+ FacingThickness);
	InVertices->Add(InStart - FacingThickness);
	InVertices->Add(InEnd	- FacingThickness);
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

