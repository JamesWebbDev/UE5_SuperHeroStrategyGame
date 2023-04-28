// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GenericEnums.h"
#include "CPP_Grid.h"
#include "DA_Attack.h"
#include "AComp_GridUser.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTurnChanged, bool, IsMyTurn);

class AAICharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INPUT_PRACTICE_API UAComp_GridUser : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAComp_GridUser();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|Gameplay")
		int32 UserIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|References")
		ACPP_Grid* Grid;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|References")
		AAICharacter* SelectedCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CPP|Server")
		E_PlayerActions ActionType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CPP|Turn Flow")
		E_PlayerActions CurrentState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CPP|Turn Flow")
		bool IsGameOver;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|Turn Flow")
		bool IsMyTurn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|Turn Flow")
		bool HasPerformedFinalInput;

	UPROPERTY(BlueprintAssignable)
		FTurnChanged OnMyTurnChanged;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "CPP|Gameplay")
		void ExecuteAttackOnServer(AAICharacter* InCharacter, UDA_Attack* InAttack, FVector InputPosition);

	// ---------- Events ---------- 
	
	UFUNCTION(BlueprintCallable, Category = "CPP|Turn Flow")
		void StateToggle(E_PlayerActions NewState);

	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "CPP|Turn Flow")
		void Event_ClientRPC_StartTurn(AAICharacter* InSelectedCharacter);

	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "CPP|Turn Flow")
		void Event_ClientRPC_EndTurn();


	UFUNCTION(BlueprintCallable, Category = "CPP|Attacking")
		void AttackStarting();

	UFUNCTION(BlueprintCallable, Category = "CPP|Attacking")
		void AttackEnding(AAICharacter* AttackingCharacter);

	UFUNCTION(BlueprintCallable, Category = "CPP|Attacking")
		void AttackThisFrame(AAICharacter* AttackingCharacter, UDA_Attack* InAttack, FVector InputPosition);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "CPP|Attacking")
		void Event_ServerRPC_StartingAttackValues(AAICharacter* AttackingCharacter, UDA_Attack* InAttack, const FVector InputPosition);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "CPP|Game Mode Relevant")
		void Event_ServerRPC_AttackStarting();

	UFUNCTION(BlueprintCallable, Server, Reliable, BlueprintAuthorityOnly, Category = "CPP|Game Mode Relevant")
		void Event_ServerRPC_AttackEnding(AAICharacter* AttackingCharacter);


	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "CPP|User Actions")
		void Event_ServerRPC_Move(const E_PlayerActions InActionType, const FVector MoveLocation);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "CPP|User Actions")
		void Event_ServerRPC_Attack(const E_PlayerActions InActionType, const TArray<FVector2D>& InAttackedPositions, const int32 InDamage);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "CPP|User Actions")
		void Event_ServerRPC_Pass(const E_PlayerActions InActionType);

	// ---------- Variables ---------- 

	UFUNCTION(BlueprintCallable, Category = "CPP|Turn Flow")
		void SetIsMyTurn(bool NewValue);

	UFUNCTION(BlueprintPure, Category = "CPP|Turn Flow")
		bool GetIsMyTurn() const;

	UFUNCTION(BlueprintCallable, Category = "CPP|Turn Flow")
		void SetHasPerformedFinalInput(bool NewValue);

	UFUNCTION(BlueprintPure, Category = "CPP|Turn Flow")
		bool GetHasPerformedFinalInput() const;



	UFUNCTION(BlueprintCallable, Category = "CPP|Gameplay")
		void SetUserIndex(int32 NewIndex);

	UFUNCTION(BlueprintPure, Category = "CPP|Gameplay")
		int32 GetUserIndex() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Gameplay")
		bool GetIsServer() const;



	UFUNCTION(BlueprintCallable, Category = "CPP|References")
		void SetSelectedCharacter(AAICharacter* InCharacter);

	UFUNCTION(BlueprintPure, Category = "CPP|References")
		AAICharacter* GetSelectedCharacter() const;

	UFUNCTION(BlueprintCallable, Category = "CPP|References")
		void SetGrid(ACPP_Grid* InGrid);

	UFUNCTION(BlueprintPure, Category = "CPP|References")
		ACPP_Grid* GetGrid() const;

		
};
