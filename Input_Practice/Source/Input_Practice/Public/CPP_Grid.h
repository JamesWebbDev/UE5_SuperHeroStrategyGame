// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "CPP_Grid.generated.h"

UCLASS()
class INPUT_PRACTICE_API ACPP_Grid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Grid();

	

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



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|Procedural Meshes")
		TArray<UProceduralMeshComponent*> MeshMoveArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|Procedural Meshes")
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
		UMaterialInstanceDynamic* CreatMaterialInstance(const FLinearColor InColour, const float InOpacity);

	UFUNCTION(BlueprintCallable, Category = "CPP|Grid")
		void CreateLine(FVector InStart, FVector InEnd, float InThickness, TArray<FVector>* InVertices, TArray<int32>* InTriangles);

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		float GetGridWidth() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		float GetGridHeight() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		bool IsTileValid(int32 Row, int32 Column) const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		float GetTileSize() const;

	UFUNCTION(BlueprintCallable, Category = "CPP")
		bool LocationToTile(const FVector Location, int32& OutRow, int32& OutColumn);

	UFUNCTION(BlueprintCallable, Category = "CPP")
		bool TileToGridWorldLocation(const int32 InRow, const int32 InColumn, const bool GetCenter, FVector2D& OutGridLocation);

};
