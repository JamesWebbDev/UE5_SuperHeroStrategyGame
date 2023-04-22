// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "GenericEnums.h"
#include "CPP_Grid.generated.h"

class AAICharacter;

UCLASS()
class INPUT_PRACTICE_API ACPP_Grid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Grid();

private:

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|Structure")
		USceneComponent* Scene;

	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|Structure")
		UProceduralMeshComponent* LineProceduralMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|Structure")
		UProceduralMeshComponent* SelectionProceduralMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|Structure")
		USceneComponent* MoveProceduralMeshParent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|Structure")
		USceneComponent* AttackProceduralMeshParent;



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CPP|Procedural Meshes")
		UMaterialInterface* Material;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "CPP|Arrays")
		TArray<UProceduralMeshComponent*> MeshMoveArray;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "CPP|Arrays")
		TArray<UProceduralMeshComponent*> MeshAttackArray;

	


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Grid")
		int32 NumRows;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Grid")
		int32 NumColumns;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Grid")
		int32 TileSize;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Grid")
		int32 NumMoveMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Grid")
		int32 NumAttackMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Grid")
		float LineThickness;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CPP|Grid")
		bool HighlightSelectedTile;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CPP|Grid")
		bool HighlightMoveableTiles;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CPP|Grid")
		bool HighlightAttackableTiles;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Lines")
		FLinearColor LineColour;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Lines")
		float LineOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Selection")
		FLinearColor SelectionColour;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Selection")
		float SelectionOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Move")
		FLinearColor MoveColour;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Move")
		float MoveOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Attack")
		FLinearColor AttackColour;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Attack")
		float AttackOpacity;

private: 

	UFUNCTION(BlueprintCallable, Category = "CPP|Grid")
		UMaterialInstanceDynamic* CreateMaterialInstance(const FLinearColor InColour, const float InOpacity);

	UFUNCTION(BlueprintCallable, Category = "CPP|Grid")
		void CreateLine(FVector InStart, FVector InEnd, float InThickness, TArray<FVector>& InVertices, TArray<int32>& InTriangles);

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		float GetGridWidth() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		float GetGridHeight() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		TArray<UProceduralMeshComponent*> GetMoveMeshChildren() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		TArray<UProceduralMeshComponent*>  GetAttackMeshChildren() const;

	UFUNCTION(BlueprintCallable, Category = "CPP|Procedural Meshes")
		void GenerateLineMeshData(TArray<FVector>& OutVertices, TArray<int32>& OutTriangles, UMaterialInstanceDynamic*& OutMaterial);

	UFUNCTION(BlueprintCallable, Category = "CPP|Procedural Meshes")
		void GenerateTileMeshData(TArray<FVector>& OutVertices, TArray<int32>& OutTriangles);

	UFUNCTION(BlueprintCallable, Category = "CPP|Procedural Meshes")
		void GenerateTileMaterial(E_TileMeshType InType, UMaterialInstanceDynamic*& OutMaterial);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		bool IsTileValid(int32 Row, int32 Column) const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		float GetTileSize() const;

	UFUNCTION(BlueprintCallable, Category = "CPP|Tile")
		bool LocationToTile(const FVector Location, int32& OutRow, int32& OutColumn);

	UFUNCTION(BlueprintCallable, Category = "CPP|Tile")
		bool TileToGridWorldLocation(const int32 InRow, const int32 InColumn, const bool GetCenter, FVector2D& OutGridLocation);

	UFUNCTION(BlueprintCallable, Category = "CPP|Tile")
		void SetHighlightedTilesState(E_PlayerActions Action, AAICharacter* InCharacter);

	UFUNCTION(BlueprintCallable, Category = "CPP|Tile")
		void SetTilePosition(UProceduralMeshComponent* Mesh, const FVector2D InTilePos, const FVector ActorLocation, const bool IsExtraTile);

	UFUNCTION(BlueprintCallable, Category = "CPP|Tile")
		void SetSelectedTileState(bool NewValue);

	UFUNCTION(BlueprintCallable, Category = "CPP|Tile")
		void SetSelectedTilePosition(int32 InRow, int32 InColumn);

	UFUNCTION(BlueprintCallable, Category = "CPP|Tile")
		void SetMoveableTilesState(bool NewValue);

	UFUNCTION(BlueprintCallable, Category = "CPP|Tile")
		void SetMoveableTilesPositions(TArray<FVector2D> InGridLocations);

	UFUNCTION(BlueprintCallable, Category = "CPP|Tile")
		void SetAttackableTilesState(bool NewValue);

	UFUNCTION(BlueprintCallable, Category = "CPP|Tile")
		void SetAttackableTilesPositions(TArray<FVector2D> InGridLocations);


};
