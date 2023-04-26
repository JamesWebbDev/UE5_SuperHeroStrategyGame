// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "CPP_TopDownGameState.h"
#include "CPP_TopDownControllerPlayer.h"
#include "Net/UnrealNetwork.h"
#include "DA_Attack.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharRoot = GetRootComponent();

	GridComponent = CreateDefaultSubobject<UAComp_Grid>(TEXT("GridComponent"));
	HealthComponent = CreateDefaultSubobject<UAComp_Health>(TEXT("HealthComponent"));
	AttackComponent = CreateDefaultSubobject<UAComp_Attack>(TEXT("AttackComponent"));

}

void AAICharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAICharacter, OwningUser);
}


// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	TDGameState = Cast<ACPP_TopDownGameState>(GetWorld()->GetGameState());

	if (TDGameState == nullptr) 
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't find 'CPP_TopDownGameState' in World!"));
		return;
	}

	//OwningUser = ;
}

void AAICharacter::MoveToDestination()
{
	if (!HasAuthority()) 
	{
		return;
	}

	const FVector ActorLocation = GetActorLocation();

	const float DistanceToDest = (TargetDestination - ActorLocation).Size();

	UCharacterMovementComponent* MovePtr = GetCharacterMovement();
	float MoveSpeed = DistanceToDest > WalkDistance ? RunSpeed : WalkSpeed;
	MovePtr->MaxWalkSpeed = MoveSpeed;

	if (DistanceToDest > SnapDistance)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), TargetDestination);
	}
	else
	{
		SetIsAtDestination(true);
	}
}

void AAICharacter::SetIsAtDestination(bool NewValue)
{
	if (IsAtDestination == NewValue)
	{
		return;
	}

	IsAtDestination = NewValue;

	if (IsAtDestination == true)
	{
		GridComponent->SetWorldPositionFromCurrentPosition();

		if (OnActionConcluded.IsBound())
			OnActionConcluded.Broadcast();
	}
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveToDestination();
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AAICharacter::Event_MultiRPC_ActionTaken_Implementation(bool InHasActed)
{
	SetHasActedThisRotation(InHasActed);
}

void AAICharacter::Event_MultiRPC_SetTargetPosition_Implementation(FVector NewPosition)
{
	TargetDestination = NewPosition;
	IsAtDestination = false;
}

void AAICharacter::Event_MultiRPC_PrepareAttack_Implementation(UDA_Attack* InAttack, const FVector InInputPos)
{
	UE_LOG(LogTemp, Display, TEXT("Received 'PrepareAttack' input from Multicast!"));

	Server_Attack = InAttack;
	Server_InputPos = InInputPos;

	const FRotator LookRotation = UKismetMathLibrary::FindRelativeLookAtRotation(CharRoot->GetComponentTransform(), InInputPos);

	CharRoot->SetWorldRotation(LookRotation);

	//BindAttackAnim(Server_Attack->GetAnimInstance());
	CharSkeletalMesh->LinkAnimClassLayers(Server_Attack->GetAnimInstance());
}

void AAICharacter::Event_MultiRPC_StopAttack_Implementation()
{
	//UnbindAttackAnim(Server_Attack->GetAnimInstance());
	if (Server_Attack)
	{
		CharSkeletalMesh->UnlinkAnimClassLayers(Server_Attack->GetAnimInstance());
	}

	Server_Attack = nullptr;
	Server_InputPos = FVector();
}

int32 AAICharacter::GetPlayerIndex() const
{
	return PlayerIndex;
}

int32 AAICharacter::GetMoveRange() const
{
	return MoveRange;
}

bool AAICharacter::GetHasActedThisRotation() const
{
	return HasActedThisRotation;
}

bool AAICharacter::IsValidGridTargetPosition(FVector2D AttackedTile, int32 Range) const
{
	AAICharacter* FoundCharacter;
	const bool CharacterIsAtTile = TDGameState->GetCharacterAtGridPosition(AttackedTile, FoundCharacter);

	if (FoundCharacter == nullptr)
	{
		return false;
	}

	const bool IsEnemy = FoundCharacter->GetPlayerIndex() != GetPlayerIndex();

	const float DistanceToTile = FVector2D::Distance(AttackedTile, GridComponent->GetCurrentLocationAtTile());
	const bool TileInRange = DistanceToTile <= Range + (Range / 10);

	if (IsEnemy && TileInRange)
	{
		return true;
	}

	return false;
}

bool AAICharacter::IsValidGridMovePosition(FVector2D MoveableTile) const
{
	AAICharacter* FoundCharacter;
	const bool CharacterIsAtTile = TDGameState->GetCharacterAtGridPosition(MoveableTile, FoundCharacter);

	const float DistanceToTile = FVector2D::Distance(MoveableTile, GridComponent->GetCurrentGridPosition());
	const bool TileInRange = DistanceToTile <= MoveRange + (MoveRange / 10);

	if (!CharacterIsAtTile && TileInRange)
	{
		return true;
	}

	return false;
}

void AAICharacter::SetHasActedThisRotation(bool NewValue)
{
	HasActedThisRotation = NewValue;

	OnActionTaken.Broadcast(HasActedThisRotation);

	if (HasActedThisRotation)
	{
		GridComponent->SetGridPosition(GridComponent->GetCurrentLocationAtTile());
	}
}

void AAICharacter::SetOwningUser(AActor* InUser)
{
	if (InUser == NULL)
	{
		return;
	}

	if (InUser->Implements<UGridUser>())
	{
		OwningUser = IGridUser::Execute_GetGridUser(InUser);
	}
	//UE_LOG(LogTemp, Warning, TEXT("WAS CAST TO GRIDUSER INVALID ON SET? %s"), OwningUser == nullptr);

	SetIsOwnerLocalPlayerController(InUser);
}

void AAICharacter::SetIsOwnerLocalPlayerController(AActor* NewOwner)
{
	AController* Control = Cast<AController>(NewOwner);

	if (Control == nullptr || Control == NULL)
	{
		IsOwnerLocalPlayerController = false;
		UE_LOG(LogTemp, Warning, TEXT("CAST TO IS LOCAL PLAYER CONTROLLER FAILED"));
		return;
	}

	IsOwnerLocalPlayerController = Control->IsLocalPlayerController();
}




