// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPP_Grid.h"
#include "AComp_Grid.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INPUT_PRACTICE_API UAComp_Grid : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAComp_Grid();

private: 

	UPROPERTY(VisibleAnywhere, Category = "CPP")
	AActor* Owner;

	UPROPERTY(VisibleAnywhere, Category = "CPP")
	ACPP_Grid* Grid;

	UPROPERTY(VisibleAnywhere, Category = "CPP")
	FVector2D GridPosition;

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		ACPP_Grid* GetGrid() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		FVector2D GetCurrentGridPosition() const;

	UFUNCTION(BlueprintPure, Category = "CPP")
		FVector2D GetCurrentLocationAtTile();

	UFUNCTION(BlueprintCallable, Category = "CPP")
		void SetGridPosition(FVector2D NewPosition);

	UFUNCTION(BlueprintCallable, Category = "CPP")
		void SetWorldPositionFromGridPosition();

	UFUNCTION(BlueprintCallable, Category = "CPP")
		void SetWorldPositionFromCurrentPosition();

	UFUNCTION(BlueprintCallable, Category = "CPP")
		void GetAttackableTiles(TArray<FVector2D>AffectedTiles_UpDir, FVector MousePosition, TArray<FVector2D>& OutTiles);

	//UFUNCTION(BlueprintCallable, Category = "CPP")
	//	TArray<FVector2D> RotatePositionsThenApplyOrigin(TArray<FVector2D> TilePositions, FVector2D Origin, DirEnum RotateDirection);
	
};
