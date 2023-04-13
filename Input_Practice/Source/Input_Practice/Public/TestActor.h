// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

UCLASS()
class INPUT_PRACTICE_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TestActor")
		uint8 bIsUp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TestActor")
		uint8 bIsInitialised;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TestActor")
		float MinHeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TestActor")
		float MaxHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestActor")
		float Max;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TestActor")
		float CurrentZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestActor")
		float Speed;


public:

	UFUNCTION(BlueprintCallable, Category = "Cone Functions")
		void TestFunc(int32 Val);

	UFUNCTION(BlueprintCallable, Category = "Cone Functions")
		void TestInputOutput(const int32 InVal, int32& OutVal);

	UFUNCTION(BlueprintCallable, Category = "Cone Functions")
		int32 TestPureFunc() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Cone Functions")
		void TestImplementableFunc(int32 Val);

	UFUNCTION(BlueprintNativeEvent, Category = "Cone Functions")
		void Explode();
		void Explode_Implementation();


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
