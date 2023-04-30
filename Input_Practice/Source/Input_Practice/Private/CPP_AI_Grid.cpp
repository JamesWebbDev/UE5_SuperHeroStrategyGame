// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AI_Grid.h"
#include "Kismet/KismetMathLibrary.h"
#include "HelperLibrary.h"

// Sets default values
ACPP_AI_Grid::ACPP_AI_Grid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UserComponent = CreateDefaultSubobject<UAComp_GridUser>(TEXT("GridUserComponent"));

	
}

// Called when the game starts or when spawned
void ACPP_AI_Grid::BeginPlay()
{
	Super::BeginPlay();
	
	GridRef = Cast<ACPP_Grid>(UserComponent->GetGrid());
	GameStateRef = Cast<ACPP_TopDownGameState>(GetWorld()->GetGameState());

	IntToDirMap.Empty();

	IntToDirMap.Add(0, E_CardinalDirection::CD_Up);
	IntToDirMap.Add(1, E_CardinalDirection::CD_Down);
	IntToDirMap.Add(2, E_CardinalDirection::CD_Right);
	IntToDirMap.Add(3, E_CardinalDirection::CD_Left);
}

// Called every frame
void ACPP_AI_Grid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_AI_Grid::InitialiseDynamicVariables()
{
	// Set Character
	SelectedChar = UserComponent->GetSelectedCharacter();

	// Set Moveable Tiles
	const int32 MoveRange = SelectedChar->GetMoveRange();
	SelectedChar->GridComponent->GetSurroundingTiles(MoveRange, false, MoveableTiles);

	// Set CharMap
	CharMap.Empty();

	const int32 EnemyIndex = UserComponent->GetUserIndex() == 1 ? 0 : 1;
	TArray<AAICharacter*> EnemyUnits = GameStateRef->GetCharactersOwnedByPlayers(EnemyIndex);

	for (AAICharacter* Char : EnemyUnits)
	{
		CharMap.Add(Char->GridComponent->GetCurrentLocationAtTile(), Char);
	}
}

FScenario ACPP_AI_Grid::FindBestScenario()
{
	TArray<FScenario> BestScenarios;
	TArray<UDA_Attack*> AttackList;
	TArray<FVector2D> AttackedTiles;
	TArray<FVector2D> CharPositions;
	CharMap.GenerateKeyArray(CharPositions);

	for (FVector2D MoveTile : MoveableTiles)
	{
		if (!SelectedChar->IsValidGridMovePosition(MoveTile))
		{
			continue;
		}

		AttackList = SelectedChar->AttackComponent->GetAttackList();

		for (UDA_Attack* Attack : AttackList)
		{
			AttackedTiles.Empty();

			if (Attack->GetIsAOE())
			{
				// Attack hits multiple tiles

				for (int i = 0; i < 4; i++)
				{
					if (!IntToDirMap.Find(i)) {
						UE_LOG(LogTemp, Error, TEXT("MISSING INDEX in DIRECTION MAP within BP_USER_AI!!"))
						continue;
					}

					E_CardinalDirection& Dir = *(IntToDirMap.Find(i));

					SelectedChar->GridComponent->GetAttackableTiles_FixedDir(Attack->GetAttackTiles(), Dir, MoveTile, AttackedTiles);

					if (!DoAnyAttacksHitCharacters(AttackedTiles)) {
						continue;
					}

					FScenario NewScenario = CalculateNewScenarioAndScore(Attack, MoveTile, AttackedTiles, Dir);

					AddNewBestScenario(BestScenarios, NewScenario);
				}
			}
			else
			{
				// Attack targets a single tile

				for (FVector2D CharPos : CharPositions)
				{
					if (!CharMap.Find(CharPos)) {
						continue;
					}

					if (!SelectedChar->IsValidGridTarget(CharPos, MoveTile, Attack->GetAttackRange())) {
						continue;
					}

					AttackedTiles.Add(CharPos);

					FScenario NewScenario = CalculateNewScenarioAndScore(Attack, MoveTile, AttackedTiles, E_CardinalDirection::CD_Up);

					AddNewBestScenario(BestScenarios, NewScenario);
				}
			}
		}
	}

	// Get MOVE Scenario in case no valid Attacks were found
	if (BestScenarios.Num() <= 0)
	{
		FScenario BackupScenario = CalculateNewMoveScenario(FindClosestCharPosition(), MoveableTiles);

		BestScenarios.Add(BackupScenario);
	}

	int32 RandIndex = UKismetMathLibrary::RandomIntegerInRange(0, BestScenarios.Num() - 1);

	return BestScenarios[RandIndex];
}

void ACPP_AI_Grid::ExecuteScenario()
{
	if (!bHasMoved)
	{
		// First Action of a New turn
		FVector2D TileLoc;
		if (GridRef->TileToGridWorldLocation(FMath::TruncToInt32((double)ChosenScenario.MovePosition.X),
			FMath::TruncToInt32((double)ChosenScenario.MovePosition.Y), true, TileLoc))
		{
			bHasMoved = true;
			FVector WorldTileVector = FVector(TileLoc.X, TileLoc.Y, 0);
			UserComponent->Event_ServerRPC_Move(E_PlayerActions::PA_Move, WorldTileVector);
		}
		

		return;
	}

	bHasMoved = false;

	if (!ChosenScenario.bIsAttacking)
	{
		UserComponent->Event_ServerRPC_Pass(E_PlayerActions::PA_Pass);
		return;
	}

	const FVector ActorLocation = SelectedChar->GetActorLocation();

	if (ChosenScenario.AttackType->GetIsAOE())
	{
		const float VectorDist = GridRef->GetTileSize();
		const float VectorDistNeg = VectorDist * -1;
		FVector VecDir;

		switch (ChosenScenario.AttackDirection)
		{
			case E_CardinalDirection::CD_Up:
			{
				VecDir = FVector(VectorDist, 0, 0);
			} break;
			case E_CardinalDirection::CD_Down:
			{
				VecDir = FVector(VectorDistNeg, 0, 0);
			} break;
			case E_CardinalDirection::CD_Right:
			{
				VecDir = FVector(0, VectorDist, 0);
			} break;
			case E_CardinalDirection::CD_Left:
			{
				VecDir = FVector(0, VectorDistNeg, 0);
			} break;
		}

		SelectedChar->Event_MultiRPC_PrepareAttack(ChosenScenario.AttackType, ActorLocation + VecDir);

		return;
	}
	else
	{
		FVector2D TargetPos = ChosenScenario.AttackPositions[0];
		FVector2D TileWorldPos2D;

		GridRef->TileToGridWorldLocation(FMath::TruncToInt32((double)TargetPos.X), FMath::TruncToInt32((double)TargetPos.Y), true, TileWorldPos2D);
		FVector WorldTileVector = FVector(TileWorldPos2D.X, TileWorldPos2D.Y, ActorLocation.Z);

		SelectedChar->Event_MultiRPC_PrepareAttack(ChosenScenario.AttackType, WorldTileVector);

		return;
	}
}

FVector2D ACPP_AI_Grid::FindClosestCharPosition()
{
	TArray<FVector2D> CharPositions;
	CharMap.GenerateKeyArray(CharPositions);

	const FVector2D CurrentPos = SelectedChar->GridComponent->GetCurrentLocationAtTile();

	const FVector2D ClosestCharPos = UHelperLibrary::GetClosestV2_ToTarget(CurrentPos, CharPositions, true);

	return ClosestCharPos;
}

void ACPP_AI_Grid::AddNewBestScenario(TArray<FScenario>& ExistingBestScenarios, FScenario InScenario)
{
	if (ExistingBestScenarios.Num() <= 0)
	{
		ExistingBestScenarios.Add(InScenario);
		return;
	}

	int ScoreThreshold = ExistingBestScenarios[0].Score;


	if (InScenario.Score < ScoreThreshold) {
		return;
	}

	if (InScenario.Score > ScoreThreshold) 
	{
		ExistingBestScenarios.Empty();
	}

	ExistingBestScenarios.Add(InScenario);
}

bool ACPP_AI_Grid::DoAnyAttacksHitCharacters(const TArray<FVector2D> AttackPositions)
{
	for (FVector2D AttackPos : AttackPositions)
	{
		if (CharMap.Find(AttackPos))
		{
			return true;
		}
	}

	return false;
}

FScenario ACPP_AI_Grid::CalculateNewScenarioAndScore(UDA_Attack* InAttack, const FVector2D InMovePos, const TArray<FVector2D> InAttackPosList, const E_CardinalDirection InAttackDir)
{
	int32 TempScore = 0;
	FVector2D ClosestAttackPos;
	TArray<FVector2D> HittingAttackTiles;

	// Calculate Move Score
	
	if (InAttackPosList.Num() == 1)
	{
		ClosestAttackPos = InAttackPosList[0];
	}
	else
	{
		for (FVector2D AttackPos : InAttackPosList)
		{
			if (CharMap.Find(AttackPos))
			{
				HittingAttackTiles.Add(AttackPos);
			}
		}

		ClosestAttackPos = UHelperLibrary::GetClosestV2_ToTarget(InMovePos, HittingAttackTiles, true);
	}

	const float AttackDist = FVector2D::Distance(InMovePos, ClosestAttackPos);

	const int32 MoveScore = (FMath::TruncToInt32((double)(AttackDist * 100)) / 10) * Multi_TargetDist;

	TempScore += MoveScore;

	// Calculate Attack Score

	for (FVector2D AttackPos : InAttackPosList)
	{
		if (!CharMap.Find(AttackPos)) 
		{
			continue;
		}

		const int32 Damage = InAttack->GetDamage();

		int32 AttackScore = Damage * Multi_DamageDealt;

		TempScore += AttackScore;

		const int32 CurrentHealth = (*CharMap.Find(AttackPos))->HealthComponent->GetCurrentHealth();

		if (CurrentHealth - Damage <= 0)
		{
			TempScore += 100000;
		}
	}

	// Create new Scenario

	FScenario Scen = FScenario();

	Scen.Score = TempScore;
	Scen.MovePosition = InMovePos;
	Scen.bIsAttacking = true;
	Scen.AttackType = InAttack;
	Scen.AttackPositions = InAttackPosList;
	Scen.AttackDirection = InAttackDir;
	

	return Scen;
}

FScenario ACPP_AI_Grid::CalculateNewMoveScenario(const FVector2D InCharPos, const TArray<FVector2D> InMoveablePosList)
{
	TArray<FVector2D> EmptyAttackList;

	FScenario NewScenario = FScenario
	{
		NewScenario.Score = 10,
		NewScenario.MovePosition = UHelperLibrary::GetClosestV2_ToTarget(InCharPos, InMoveablePosList, true),
	};

	return NewScenario;
}

