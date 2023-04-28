// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AComp_Grid.h"
#include "AComp_Health.h"
#include "AComp_Attack.h"
#include "AComp_GridUser.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AICharacter.generated.h"

// ------- FORWARD DECLARATIONS ---------
class ACPP_TopDownGameState;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActionTakenDelegate, bool, HasActed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionConcluded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSelected);



UCLASS()
class INPUT_PRACTICE_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|References")
		ACPP_TopDownGameState* TDGameState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "CPP|References")
		UAComp_GridUser* OwningUser;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CPP|Components")
		USkeletalMeshComponent* CharSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|Components")
		USceneComponent* CharRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|Components")
		UAComp_Grid* GridComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|Components")
		UAComp_Health* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|Components")
		UAComp_Attack* AttackComponent;

	


	UPROPERTY(VisibleDefaultsOnly, BlueprintAssignable, Category = "CPP|Events")
		FActionTakenDelegate OnActionTaken;
	UPROPERTY(VisibleDefaultsOnly, BlueprintAssignable, Category = "CPP|Events")
		FActionConcluded OnActionConcluded;
	UPROPERTY(VisibleDefaultsOnly, BlueprintAssignable, Category = "CPP|Events")
		FSelected OnSelected;


protected: 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Movement", meta = (ToolTip = "Defines the radius in which this character can move in a grid."))
		int32 MoveRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Movement")
		int32 RunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Movement")
		int32 WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Movement", meta = (ToolTip = "Defines the distance from the destination needed to start walking."))
		int32 WalkDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Movement", meta = (ToolTip = "Distance to which player snaps to destination."))
		int32 SnapDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CPP|Movement")
		FVector  TargetDestination;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CPP|Movement")
		bool  IsAtDestination;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Gameplay")
		int32 PlayerIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP|Gameplay", meta=(Tooltip = "Greater the value, the MORE priority this unit has to move!"))
		int32 TurnPriority;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|Gameplay")
		bool  HasActedThisRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CPP|Gameplay")
		bool  IsOwnerLocalPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CPP|Server Relevant")
		UDA_Attack* Server_Attack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CPP|Server Relevant")
		FVector Server_InputPos;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "CPP|Movement")
		void MoveToDestination();

	UFUNCTION(BlueprintCallable, Category = "CPP|Movement")
		void SetIsAtDestination(bool NewValue);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Blueprint Events

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "CPP|Turn Flow")
		void InitialiseGameValues(const AActor* InActor);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "CPP|Grid")
		void ActivateMoveableTiles();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "CPP|Grid")
		void ActivateAttackableTiles();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "CPP|Turn Flow")
		void AttackPositions(const TArray<FVector2D>& AttackedTiles, const int32 Damage);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "CPP|BP Accessible")
		void BindAttackAnim(TSubclassOf<class UAnimInstance> InAnim);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "CPP|BP Accessible")
		void UnbindAttackAnim(TSubclassOf<class UAnimInstance> InAnim);

	// Networked Events

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "CPP|Turn Flow")
		void Event_MultiRPC_ActionTaken(const bool InHasActed);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "CPP|Turn Flow")
		void Event_MultiRPC_SetTargetPosition(const FVector NewPosition);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "CPP|Turn Flow")
		void Event_MultiRPC_PrepareAttack(UDA_Attack* InAttack, const FVector InInputPos);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "CPP|Turn Flow")
		void Event_MultiRPC_StopAttack();

	// Functions


	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		int32 GetPlayerIndex() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		int32 GetMoveRange() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		int32 GetPriority() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Constants")
		bool GetHasActedThisRotation() const;

	UFUNCTION(BlueprintPure, Category = "CPP|Gameplay")
		bool IsValidGridTargetPosition(FVector2D AttackedTile, int32 Range) const;

	UFUNCTION(BlueprintPure, Category = "CPP|Gameplay")
		bool IsValidGridTarget(AAICharacter* InEnemyTarget, FVector2D AttackedTile, FVector2D AttackerTile, int32 InRange) const;

	UFUNCTION(BlueprintPure, Category = "CPP|Gameplay")
		bool IsValidGridMovePosition(FVector2D MoveableTile) const;

	UFUNCTION(BlueprintCallable, Category = "CPP|Gameplay")
		void SetHasActedThisRotation(bool NewValue);

	UFUNCTION(BlueprintCallable, Category = "CPP|Gameplay")
		void SetOwningUser(AActor* InUser);

	UFUNCTION(BlueprintCallable, Category = "CPP|Gameplay")
		void SetIsOwnerLocalPlayerController(AActor* NewOwner);

};
