// Fill out your copyright notice in the Description page of Project Settings.


#include "AComp_GridUser.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CPP_TopDownGameMode.h"
#include "AICharacter.h"


// Sets default values for this component's properties
UAComp_GridUser::UAComp_GridUser()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAComp_GridUser::BeginPlay()
{
	Super::BeginPlay();

	Grid = Cast<ACPP_Grid>(UGameplayStatics::GetActorOfClass(GetWorld(), ACPP_Grid::StaticClass()));
	
}


// Called every frame
void UAComp_GridUser::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAComp_GridUser::ExecuteAttackOnServer(AAICharacter* InCharacter, UDA_Attack* InAttack, FVector InputPosition)
{
	if (InAttack->GetIsAOE())
	{
		// AOE Attack

		TArray<FVector2D> OutAttackTiles;
		InCharacter->GridComponent->GetAttackableTiles(InAttack->GetAttackTiles(), InputPosition, OutAttackTiles);

		Event_ServerRPC_Attack(E_PlayerActions::PA_Attack, OutAttackTiles, InAttack->GetDamage());
	}
	else
	{
		// Targeted Attack

		TArray<FVector2D> OutAttackTiles;
		FVector2D GridPos;
		FVector WorldPos;

		Grid->VectorToTile(InputPosition, WorldPos, GridPos);
		OutAttackTiles.Add(GridPos);

		Event_ServerRPC_Attack(E_PlayerActions::PA_Attack, OutAttackTiles, InAttack->GetDamage());
	}
}

void UAComp_GridUser::StateToggle(E_PlayerActions NewState)
{
	CurrentState = NewState;

	Grid->SetHighlightedTilesState(CurrentState, SelectedCharacter);
}

void UAComp_GridUser::AttackStarting()
{
	Event_ServerRPC_AttackStarting();
}

void UAComp_GridUser::AttackEnding(AAICharacter* AttackingCharacter)
{
	Event_ServerRPC_AttackEnding(AttackingCharacter);
}

void UAComp_GridUser::AttackThisFrame(AAICharacter* AttackingCharacter, UDA_Attack* InAttack, FVector InputPosition)
{
	ExecuteAttackOnServer(AttackingCharacter, InAttack, InputPosition);
}


void UAComp_GridUser::Event_ServerRPC_AttackStarting_Implementation()
{
	ACPP_TopDownGameMode* GameMode = Cast<ACPP_TopDownGameMode>(GetWorld()->GetAuthGameMode());

	GameMode->EndUserTurn(E_PlayerActions::PA_Pass);
}

void UAComp_GridUser::Event_ServerRPC_AttackEnding_Implementation(AAICharacter* AttackingCharacter)
{
	AttackingCharacter->PrepareToStopAttack();

	ACPP_TopDownGameMode* GameMode = Cast<ACPP_TopDownGameMode>(GetWorld()->GetAuthGameMode());

	GameMode->CharacterFinalActionHasConcluded();
}

void UAComp_GridUser::Event_ServerRPC_StartingAttackValues_Implementation(AAICharacter* AttackingCharacter, const UDA_Attack* InAttack, const FVector InputPosition)
{
	AttackingCharacter->PrepareToStartAttack(InAttack, InputPosition);
}

void UAComp_GridUser::Event_ClientRPC_StartTurn_Implementation(AAICharacter* InSelectedCharacter)
{
	SelectedCharacter = InSelectedCharacter;
	Grid->SetMoveableTilesState(true);
	Grid->SetSelectedTileState(true);
	SetIsMyTurn(true);
	SelectedCharacter->ActivateMoveableTiles();
}

void UAComp_GridUser::Event_ClientRPC_EndTurn_Implementation()
{
	SelectedCharacter = nullptr;
	Grid->SetMoveableTilesState(false);
	Grid->SetSelectedTileState(false);
	SetIsMyTurn(false);
}

void UAComp_GridUser::Event_ServerRPC_Move_Implementation(const E_PlayerActions InActionType, const FVector MoveLocation)
{
	ACPP_TopDownGameMode* GameMode = Cast<ACPP_TopDownGameMode>(GetWorld()->GetAuthGameMode());
	AAICharacter* TempSelectedCharacter = GameMode->GetSelectedCharacter();

	TempSelectedCharacter->Event_MultiRPC_SetTargetPosition(MoveLocation);
	TempSelectedCharacter->Event_MultiRPC_ActionTaken(false);

	GameMode->EndUserTurn(InActionType);
}

void UAComp_GridUser::Event_ServerRPC_Attack_Implementation(const E_PlayerActions InActionType, const TArray<FVector2D>& InAttackedPositions, const int32 InDamage)
{
	ACPP_TopDownGameMode* GameMode = Cast<ACPP_TopDownGameMode>(GetWorld()->GetAuthGameMode());
	AAICharacter* TempSelectedCharacter = GameMode->GetSelectedCharacter();

	TempSelectedCharacter->AttackPositions(InAttackedPositions, InDamage);
	TempSelectedCharacter->Event_MultiRPC_ActionTaken(true);

	GameMode->EndUserTurn(InActionType);
}

void UAComp_GridUser::Event_ServerRPC_Pass_Implementation(const E_PlayerActions InActionType)
{
	ACPP_TopDownGameMode* GameMode = Cast<ACPP_TopDownGameMode>(GetWorld()->GetAuthGameMode());
	AAICharacter* TempSelectedCharacter = GameMode->GetSelectedCharacter();

	TempSelectedCharacter->Event_MultiRPC_ActionTaken(true);

	GameMode->EndUserTurn(InActionType);
}

void UAComp_GridUser::SetIsMyTurn(bool NewValue)
{
	HasPerformedFinalInput = false;
	IsMyTurn = NewValue;

	if (OnMyTurnChanged.IsBound())
	{
		OnMyTurnChanged.Broadcast(IsMyTurn);
	}
}

bool UAComp_GridUser::GetIsMyTurn() const
{
	return IsMyTurn;
}

void UAComp_GridUser::SetHasPerformedFinalInput(bool NewValue)
{
	HasPerformedFinalInput = NewValue;
}

bool UAComp_GridUser::GetHasPerformedFinalInput() const
{
	return HasPerformedFinalInput;
}

void UAComp_GridUser::SetUserIndex(int32 NewIndex)
{
	UserIndex = NewIndex;
}

int32 UAComp_GridUser::GetUserIndex() const
{
	return UserIndex;
}

bool UAComp_GridUser::GetIsServer() const
{
	return UKismetSystemLibrary::IsServer(this->_getUObject());
}

void UAComp_GridUser::SetSelectedCharacter(AAICharacter* InCharacter)
{
	SelectedCharacter = InCharacter;
}

AAICharacter* UAComp_GridUser::GetSelectedCharacter() const
{
	return SelectedCharacter;
}

void UAComp_GridUser::SetGrid(ACPP_Grid* InGrid)
{
	Grid = InGrid;
}

ACPP_Grid* UAComp_GridUser::GetGrid() const
{
	return Grid;
}


