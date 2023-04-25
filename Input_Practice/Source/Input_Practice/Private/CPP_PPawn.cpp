// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PPawn.h"

// Sets default values
ACPP_PPawn::ACPP_PPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UserComponent = CreateDefaultSubobject<UAComp_GridUser>(TEXT("GridUserComponent"));
}

// Called when the game starts or when spawned
void ACPP_PPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_PPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPP_PPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

