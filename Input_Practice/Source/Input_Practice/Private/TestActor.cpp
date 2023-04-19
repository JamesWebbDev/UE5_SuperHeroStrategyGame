// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"
#include "ProceduralMeshComponent.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	TestProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TestMesh"));

	Scene->SetupAttachment(GetRootComponent());
	Mesh->SetupAttachment(Scene);
	TestProceduralMesh->SetupAttachment(Scene);

	TestProceduralMesh->CreateMeshSection(0, TestVertices, TestTriangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);

	Speed		   = 50;
	Max			   = 200;
	bIsUp		   = true;
	bIsInitialised = false;
}



// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	
	TestImplementableFunc(76);

	const FVector Location = GetActorLocation();

	CurrentZ		= Location.Z;
	MinHeight		= Location.Z;
	MaxHeight		= Location.Z + Max;
	bIsInitialised	= true;
}

void ATestActor::TestFunc(int32 Val) 
{
	UE_LOG(LogTemp, Warning, TEXT("Test Func Called with value %d"), Val);
}

void ATestActor::TestInputOutput(const int32 InVal, int32& OutVal)
{
	OutVal = InVal;
}

int32 ATestActor::TestPureFunc()const
{
	return 25;
}

void ATestActor::Explode_Implementation()
{
	this->Destroy();
}


// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float Val = DeltaTime * Speed;

	if (bIsUp) {
		CurrentZ += Val;
	}
	else {
		CurrentZ -= Val;
	}

	FVector Location = GetActorLocation();
	Location.Z = CurrentZ;

	SetActorLocation(Location);

	if (CurrentZ > MaxHeight) {
		bIsUp = false;
		//Explode();
	}
	else if (CurrentZ < MinHeight) {
		bIsUp = true;
	}

}

