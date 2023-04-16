// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GridComponent = CreateDefaultSubobject<UAComp_Grid>(TEXT("GridComponent"));
	HealthComponent = CreateDefaultSubobject<UAComp_Health>(TEXT("HealthComponent"));
	AttackComponent = CreateDefaultSubobject<UAComp_Attack>(TEXT("AttackComponent"));

}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

int32 AAICharacter::GetPlayerIndex() const
{
	return PlayerIndex;
}

int32 AAICharacter::GetSpeed() const
{
	return Speed;
}

bool AAICharacter::GetHasActedThisRotation() const
{
	return HasActedThisRotation;
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



