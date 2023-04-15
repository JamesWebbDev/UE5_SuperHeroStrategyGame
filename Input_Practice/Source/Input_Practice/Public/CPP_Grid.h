// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Grid.generated.h"

UCLASS()
class INPUT_PRACTICE_API ACPP_Grid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Grid();

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		bool IsTileValid(int32 Row, int32 Column) const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		float GetGridWidth() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		float GetGridHeight() const;

	UFUNCTION(BlueprintCallable, Category = "CPP")
		bool LocationToTile(const FVector Location, int32& InRow, int32& InColumn);

	UFUNCTION(BlueprintCallable, Category = "CPP")
		bool TileToGridWorldLocation(const int32 InRow, const int32 InColumn, const bool GetCenter, FVector2D& OutGridLocation);

};
