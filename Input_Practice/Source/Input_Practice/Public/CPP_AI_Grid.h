// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridUser.h"
#include "GenericStructs.h"
#include "AICharacter.h"
#include "CPP_Grid.h"
#include "CPP_TopDownGameState.h"
#include "CPP_AI_Grid.generated.h"

UCLASS()
class INPUT_PRACTICE_API ACPP_AI_Grid : public AActor, public IGridUser
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_AI_Grid();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|Components")
		UAComp_GridUser* UserComponent;

	

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "CPP|References (On Begin Play)")
		ACPP_Grid* GridRef;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "CPP|References (On Begin Play)")
		ACPP_TopDownGameState* GameStateRef;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "CPP|Dynamic")
		AAICharacter* SelectedChar;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "CPP|Dynamic")
		TArray<FVector2D> MoveableTiles;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "CPP|Dynamic")
		TMap<FVector2D, AAICharacter*> CharMap;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "CPP|Constants")
		TMap<int, E_CardinalDirection> IntToDirMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Constants")
		int32 Multi_TargetDist = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Constants")
		int32 Multi_DamageDealt = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Constants")
		int32 Multi_AttackCost = 1;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "CPP|Gameplay", meta = (DisplayName="HasMoved"))
		bool bHasMoved;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "CPP|Gameplay")
		FScenario ChosenScenario;



	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "CPP|Gameplay")
		void StartNewTurn();

protected:

	UFUNCTION(BlueprintCallable, Category = "CPP|Scenario")
		void InitialiseDynamicVariables();

	UFUNCTION(BlueprintCallable, Category = "CPP|Scenario")
		FScenario FindBestScenario();

	UFUNCTION(BlueprintCallable, Category = "CPP|Scenario")
		void ExecuteScenario();

	UFUNCTION(BlueprintCallable, Category = "CPP|Scenario")
		FVector2D FindClosestCharPosition();

	UFUNCTION(BlueprintCallable, Category = "CPP|Scenario")
		void AddNewBestScenario(TArray<FScenario>& ExistingBestScenarios, FScenario InScenario);

	UFUNCTION(BlueprintPure, Category = "CPP|Scenario")
		bool DoAnyAttacksHitCharacters(const TArray<FVector2D> AttackPositions);

	UFUNCTION(BlueprintCallable, Category = "CPP|Scenario")
		FScenario CalculateNewScenarioAndScore(UDA_Attack* InAttack, const FVector2D InMovePos, const TArray<FVector2D> InAttackPosList, const E_CardinalDirection InAttackDir);

	UFUNCTION(BlueprintCallable, Category = "CPP|Scenario")
		FScenario CalculateNewMoveScenario(const FVector2D InCharPos, const TArray<FVector2D> InMoveablePosList);
};
